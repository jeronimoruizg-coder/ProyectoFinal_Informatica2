#include "bomba.h"

Bomba::Bomba(int filaInicial, int columnaInicial)
{
    fila = filaInicial;
    columna = columnaInicial;

    // La bomba ocupa toda su casilla
    ancho = Mapa::TAM_CASILLA;
    alto = Mapa::TAM_CASILLA;
    x = columna * Mapa::TAM_CASILLA;
    y = fila * Mapa::TAM_CASILLA;

    tiempoMecha = 2.5;
    alcance = 2;

    explotada = false;
    tiempoExplosion = 0;
}

// Funcion heredada de Objetos
void Bomba::actualizar()
{
    actualizar(0.016);
}

void Bomba::actualizar(float intervaloTiempo)
{
    // Si ya exploto, avanzamos el tiempo de la animacion
    if (explotada)
    {
        tiempoExplosion += intervaloTiempo;
        return;
    }

    // Si no, se va consumiendo la mecha
    tiempoMecha -= intervaloTiempo;

    if (tiempoMecha < 0)
    {
        tiempoMecha = 0;
    }
}

bool Bomba::debeExplotar() const
{
    return !explotada && tiempoMecha <= 0;
}

// Calcula la explosion en cruz y rompe las paredes que alcance
void Bomba::explotar(Mapa &mapa)
{
    if (explotada)
    {
        return;
    }

    explotada = true;
    tiempoExplosion = 0;

    // La casilla de la bomba siempre tiene fuego
    agregarFuego(fila, columna);

    // Direcciones: arriba, abajo, izquierda, derecha
    int cambioFila[4] = {-1, 1, 0, 0};
    int cambioColumna[4] = {0, 0, -1, 1};

    for (int d = 0; d < 4; d++)
    {
        for (int paso = 1; paso <= alcance; paso++)
        {
            int f = fila + cambioFila[d] * paso;
            int c = columna + cambioColumna[d] * paso;
            int tipo = mapa.getCasilla(f, c);

            // Las paredes fijas y la puerta detienen el fuego
            if (tipo == Mapa::PARED_FIJA || tipo == Mapa::PUERTA)
            {
                break;
            }

            // La pared rompible se destruye y ahi se detiene el fuego
            if (tipo == Mapa::PARED_ROMPIBLE)
            {
                mapa.romperPared(f, c);
                agregarFuego(f, c);
                break;
            }

            // En un pasillo el fuego sigue avanzando
            agregarFuego(f, c);
        }
    }
}

void Bomba::agregarFuego(int filaFuego, int columnaFuego)
{
    filasFuego.push_back(filaFuego);
    columnasFuego.push_back(columnaFuego);
}

bool Bomba::haExplotado() const
{
    return explotada;
}

// La animacion de explosion tiene 8 fotogramas de 0.08 s
bool Bomba::terminoExplosion() const
{
    return explotada && tiempoExplosion >= 0.64;
}

int Bomba::getFotogramaExplosion() const
{
    int fotograma = int(tiempoExplosion / 0.08);

    // Evitamos pedir un fotograma que no existe
    if (fotograma > 7)
    {
        fotograma = 7;
    }

    return fotograma;
}

bool Bomba::hayFuegoEn(int filaConsulta, int columnaConsulta) const
{
    if (!explotada)
    {
        return false;
    }

    for (int i = 0; i < (int)filasFuego.size(); i++)
    {
        if (filasFuego[i] == filaConsulta && columnasFuego[i] == columnaConsulta)
        {
            return true;
        }
    }

    return false;
}

int Bomba::getCantidadFuego() const
{
    return filasFuego.size();
}

int Bomba::getFilaFuego(int indice) const
{
    return filasFuego[indice];
}

int Bomba::getColumnaFuego(int indice) const
{
    return columnasFuego[indice];
}

int Bomba::getFila() const
{
    return fila;
}

int Bomba::getColumna() const
{
    return columna;
}

float Bomba::getTiempoMecha() const
{
    return tiempoMecha;
}