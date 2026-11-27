#include "mono.h"
#include <cstdlib>

Mono::Mono(int filaInicial, int columnaInicial)
{
    ancho = 40;
    alto = 40;
    velocidad = 85;
    direccion = 1;
    vivo = true;

    fila = filaInicial;
    columna = columnaInicial;
    filaDestino = fila;
    columnaDestino = columna;

    // Arranca centrado en su casilla
    x = columna * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    y = fila * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;

    dirFila = 0;
    dirColumna = -1;

    tiempoRecarga = 1.0;
    estrategia = 0;
    lanzamientoPendiente = false;
    filaObjetivo = 0;
    columnaObjetivo = 0;
    distanciaAnteriorDexter = 100;
}

void Mono::actualizar(float intervaloTiempo, Mapa &mapa, const std::vector<Bomba*> &bombas,
                      int filaDexter, int columnaDexter)
{
    // Si ya murio (por ejemplo, lo embistio el armadillo) no hace nada
    if (!vivo)
    {
        return;
    }

    if (tiempoRecarga > 0)
    {
        tiempoRecarga -= intervaloTiempo;
    }

    // Solo decide cuando esta en el centro de una casilla
    if (!avanzarHaciaDestino(intervaloTiempo))
    {
        return;
    }

    // PERCIBIR: distancia a Dexter (en casillas)
    int distancia = abs(filaDexter - fila) + abs(columnaDexter - columna);

    // RAZONAR Y ACTUAR, en orden de prioridad:

    // 1. Si una bomba lo puede alcanzar, huye
    if (estaEnPeligro(mapa, bombas, fila, columna))
    {
        moverAVecino(mapa, bombas, filaDexter, columnaDexter, HUIR_BOMBA);
    }
    // 2. Si Dexter esta cerca, ataca o se acomoda
    else if (distancia <= 7)
    {
        if (intentarLanzar(mapa, bombas, filaDexter, columnaDexter, distancia))
        {
            // Se queda quieto mientras lanza
        }
        else if (distancia <= 1)
        {
            moverAVecino(mapa, bombas, filaDexter, columnaDexter, ALEJARSE);
        }
        else if (distancia > 4 || (filaDexter != fila && columnaDexter != columna))
        {
            moverAVecino(mapa, bombas, filaDexter, columnaDexter, ACERCARSE);
        }
        // Esta alineado pero no puede lanzar (recargando o sin donde caer):
        // se mueve para buscar otra posicion en vez de quedarse quieto
        else
        {
            moverAVecino(mapa, bombas, filaDexter, columnaDexter, PASEAR);
        }
    }
    // 3. Si no pasa nada, pasea
    else
    {
        moverAVecino(mapa, bombas, filaDexter, columnaDexter, PASEAR);
    }

    distanciaAnteriorDexter = distancia;
}

//------------------------------------------------ Percibir ----------------------------------//

bool Mono::hayBombaEn(const std::vector<Bomba*> &bombas, int f, int c)
{
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        if (!bombas[i]->haExplotado() &&
            bombas[i]->getFila() == f && bombas[i]->getColumna() == c)
        {
            return true;
        }
    }
    return false;
}

// Una casilla esta libre si no es pared ni tiene una bomba
bool Mono::estaLibre(Mapa &mapa, const std::vector<Bomba*> &bombas, int f, int c)
{
    return !mapa.esPared(f, c) && !hayBombaEn(bombas, f, c);
}

// Revisa si la explosion en cruz de alguna bomba llegaria a esa casilla
bool Mono::estaEnPeligro(Mapa &mapa, const std::vector<Bomba*> &bombas, int f, int c)
{
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        Bomba *bomba = bombas[i];

        // El fuego que ya esta encendido tambien es peligro
        if (bomba->hayFuegoEn(f, c))
        {
            return true;
        }

        if (bomba->haExplotado())
        {
            continue;
        }

        // Reacciona tarde: no le presta atencion a una bomba recien puesta
        if (bomba->getTiempoMecha() > 1.5)
        {
            continue;
        }

        // Tiene que estar en la misma fila o columna de la bomba
        if (bomba->getFila() != f && bomba->getColumna() != c)
        {
            continue;
        }

        int distancia = abs(bomba->getFila() - f) + abs(bomba->getColumna() - c);
        if (distancia > bomba->getAlcance())
        {
            continue;
        }

        // Si hay una pared en medio, el fuego no llega
        int pasoFila = 0;
        int pasoColumna = 0;
        if (f > bomba->getFila()) pasoFila = 1;
        if (f < bomba->getFila()) pasoFila = -1;
        if (c > bomba->getColumna()) pasoColumna = 1;
        if (c < bomba->getColumna()) pasoColumna = -1;

        bool tapado = false;
        for (int k = 1; k < distancia; k++)
        {
            if (mapa.esPared(bomba->getFila() + pasoFila * k, bomba->getColumna() + pasoColumna * k))
            {
                tapado = true;
            }
        }

        if (!tapado)
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------ Razonar y actuar --------------------------//

// La estrategia decide en que momento lanzar.
bool Mono::intentarLanzar(Mapa &mapa, const std::vector<Bomba*> &bombas,
                          int filaDexter, int columnaDexter, int distancia)
{
    // No lanza otra hasta que la anterior termine de explotar
    if (tiempoRecarga > 0 || tieneSemillaActiva(bombas))
    {
        return false;
    }

    // Dexter tiene que estar en su fila o columna, a 5 casillas o menos
    if ((filaDexter != fila && columnaDexter != columna) || distancia < 1 || distancia > 5)
    {
        return false;
    }

    // Direccion hacia Dexter
    int pasoFila = 0;
    int pasoColumna = 0;
    if (filaDexter > fila) pasoFila = 1;
    if (filaDexter < fila) pasoFila = -1;
    if (columnaDexter > columna) pasoColumna = 1;
    if (columnaDexter < columna) pasoColumna = -1;

    // La semilla tiene alcance 1, asi que le pega a Dexter si cae
    // en su casilla, una antes o una despues. La estrategia decide
    // cual de las tres intenta primero:
    //   0 = directo a Dexter, 1 = delante de el, 2 = detras de el
    int saltos[3];
    if (estrategia == 1)
    {
        saltos[0] = distancia - 1;
        saltos[1] = distancia;
        saltos[2] = distancia + 1;
    }
    else if (estrategia == 2)
    {
        saltos[0] = distancia + 1;
        saltos[1] = distancia;
        saltos[2] = distancia - 1;
    }
    else
    {
        saltos[0] = distancia;
        saltos[1] = distancia - 1;
        saltos[2] = distancia + 1;
    }

    for (int i = 0; i < 3; i++)
    {
        // Muy cerca lo alcanzaria su propio fuego, muy lejos no llega
        if (saltos[i] < 2 || saltos[i] > 4)
        {
            continue;
        }

        int filaCae = fila + pasoFila * saltos[i];
        int columnaCae = columna + pasoColumna * saltos[i];

        // Solo puede caer en un pasillo sin bomba (las paredes y las bombas son solidas)
        if (estaLibre(mapa, bombas, filaCae, columnaCae))
        {
            lanzamientoPendiente = true;
            filaObjetivo = filaCae;
            columnaObjetivo = columnaCae;
            tiempoRecarga = 2.5;
            return true;
        }
    }

    // No encontro donde lanzar
    return false;
}

// Revisa si su semilla sigue en el mapa (volando, en el suelo o explotando)
bool Mono::tieneSemillaActiva(const std::vector<Bomba*> &bombas)
{
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        if (bombas[i]->esSemillaDelMono())
        {
            return true;
        }
    }
    return false;
}

// Elige una casilla vecina segun lo que quiera hacer
void Mono::moverAVecino(Mapa &mapa, const std::vector<Bomba*> &bombas,
                        int filaDexter, int columnaDexter, int modo)
{
    int cambioFila[4] = {-1, 1, 0, 0};
    int cambioColumna[4] = {0, 0, -1, 1};

    int mejor = -1;
    int mejorPuntaje = -1000;
    int distanciaActual = abs(filaDexter - fila) + abs(columnaDexter - columna);

    for (int d = 0; d < 4; d++)
    {
        int f = fila + cambioFila[d];
        int c = columna + cambioColumna[d];

        if (!estaLibre(mapa, bombas, f, c))
        {
            continue;
        }

        bool peligro = estaEnPeligro(mapa, bombas, f, c);
        int distancia = abs(filaDexter - f) + abs(columnaDexter - c);
        int puntaje = 0;

        if (modo == HUIR_BOMBA)
        {
            // Lo importante es salir del peligro
            puntaje = peligro ? 0 : 10;
        }
        else
        {
            // En los otros modos nunca se mete al peligro
            if (peligro)
            {
                continue;
            }

            if (modo == ACERCARSE)
            {
                puntaje = distanciaActual - distancia;
            }
            else if (modo == ALEJARSE)
            {
                puntaje = distancia - distanciaActual;
            }
            else
            {
                // Pasear: prefiere seguir derecho
                puntaje = rand() % 3;
                if (cambioFila[d] == dirFila && cambioColumna[d] == dirColumna)
                {
                    puntaje += 3;
                }
            }
        }

        if (puntaje > mejorPuntaje)
        {
            mejorPuntaje = puntaje;
            mejor = d;
        }
    }

    // Sin salida: se queda donde esta
    if (mejor == -1)
    {
        return;
    }

    dirFila = cambioFila[mejor];
    dirColumna = cambioColumna[mejor];
    filaDestino = fila + dirFila;
    columnaDestino = columna + dirColumna;
}

// Mueve al mono hacia el centro de su casilla destino (true = ya llego)
bool Mono::avanzarHaciaDestino(float intervaloTiempo)
{
    float destinoX = columnaDestino * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    float destinoY = filaDestino * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;
    float paso = velocidad * intervaloTiempo;

    if (x < destinoX)
    {
        x += paso;
        if (x > destinoX) x = destinoX;
    }
    else if (x > destinoX)
    {
        x -= paso;
        if (x < destinoX) x = destinoX;
    }

    if (y < destinoY)
    {
        y += paso;
        if (y > destinoY) y = destinoY;
    }
    else if (y > destinoY)
    {
        y -= paso;
        if (y < destinoY) y = destinoY;
    }

    if (x == destinoX && y == destinoY)
    {
        fila = filaDestino;
        columna = columnaDestino;
        return true;
    }
    return false;
}

//------------------------------------------------ Consultas ---------------------------------//

bool Mono::quiereLanzar() const
{
    return lanzamientoPendiente;
}

int Mono::getFilaObjetivo() const
{
    return filaObjetivo;
}

int Mono::getColumnaObjetivo() const
{
    return columnaObjetivo;
}

void Mono::confirmarLanzamiento()
{
    lanzamientoPendiente = false;
}

void Mono::setEstrategia(int nuevaEstrategia)
{
    estrategia = nuevaEstrategia;
}

int Mono::getEstrategia() const
{
    return estrategia;
}

int Mono::getFila() const
{
    return fila;
}

int Mono::getColumna() const
{
    return columna;
}

void Mono::morir()
{
    vivo = false;
}

bool Mono::estaVivo() const
{
    return vivo;
}