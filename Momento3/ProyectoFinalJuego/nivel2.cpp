#include "nivel2.h"
#include <cstdlib>
#include <ctime>

// La inteligencia del mono tiene 3 estrategias de ataque
Nivel2::Nivel2(bool dificultad): inteligenciaMono(3)
{
    dificil = dificultad;
    // Numeros aleatorios para el recorrido de los armadillos
    srand(time(0));
    reiniciar();
}

Nivel2::~Nivel2()
{
    // Liberamos las bombas que queden al cerrar el nivel
    eliminarTodasLasBombas();
    eliminarTodosLosEnemigos();
    eliminarBeneficiosYDisparos();
}

void Nivel2::reiniciar()
{
    eliminarTodasLasBombas();
    eliminarBeneficiosYDisparos();
    mapa.reiniciar();
    dexter.iniciarNivel2();
    crearEnemigos();
    crearBeneficios();
}

void Nivel2::actualizar(int dirX, int dirY, float intervaloTiempo)
{
    // Si la partida termino no actualizamos nada
    if (haPerdido() || haGanado())
    {
        return;
    }

    moverDexter(dirX, dirY, intervaloTiempo);
    empujarDexter(intervaloTiempo);

    dexter.actualizarInvulnerabilidad(intervaloTiempo);
    dexter.actualizarPoderes(intervaloTiempo);

    actualizarBombas(intervaloTiempo);
    actualizarBeneficios();
    actualizarDisparos(intervaloTiempo);
    actualizarNieblas(intervaloTiempo);
    actualizarArmadillos(intervaloTiempo);
    actualizarMonos(intervaloTiempo);

    revisarPuerta();
}

//--------------------------------------------------- Bombas ----------------------------------//

void Nivel2::ponerBomba()
{
    int fila = getFilaDexter();
    int columna = getColumnaDexter();

    for (int i = 0; i < (int)bombas.size(); i++)
    {
        // Solo se puede tener una bomba con la mecha encendida
        // (las semillas del mono no cuentan)
        if (!bombas[i]->haExplotado() && !bombas[i]->esSemillaDelMono())
        {
            return;
        }

        // Tampoco se pone una bomba donde todavia hay otra explotando
        if (bombas[i]->getFila() == fila && bombas[i]->getColumna() == columna)
        {
            return;
        }
    }

    bombas.push_back(new Bomba(fila, columna));
}

void Nivel2::actualizarBombas(float intervaloTiempo)
{
    // Primero avanzamos mechas y hacemos explotar las que toque
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        bombas[i]->actualizar(intervaloTiempo);

        if (bombas[i]->debeExplotar())
        {
            explotarBomba(i);
        }
    }

    // Reaccion en cadena: el fuego hace explotar otras bombas
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        if (!bombas[i]->haExplotado() &&
            hayFuegoEnCasilla(bombas[i]->getFila(), bombas[i]->getColumna()))
        {
            explotarBomba(i);
        }
    }

    // Si Dexter esta en el fuego pierde una vida
    // (la invulnerabilidad evita que pierda varias seguidas)
    if (hayFuegoEnCasilla(getFilaDexter(), getColumnaDexter()))
    {
        dexter.recibirDano();
    }

    // Borramos las bombas que ya terminaron de explotar
    int i = 0;
    while (i < (int)bombas.size())
    {
        if (bombas[i]->terminoExplosion())
        {
            eliminarBomba(i);
        }
        else
        {
            i++;
        }
    }
}

// Igual que con las paredes, se mueve un eje a la vez:
// si al moverse queda encima de una bomba solida, se devuelve
void Nivel2::moverDexter(int dirX, int dirY, float intervaloTiempo)
{
    float anteriorX = dexter.getX();
    dexter.mover(dirX, 0, intervaloTiempo, mapa);
    if (dexterChocaConBomba())
    {
        dexter.setPosicion(anteriorX, dexter.getY());
    }

    float anteriorY = dexter.getY();
    dexter.mover(0, dirY, intervaloTiempo, mapa);
    if (dexterChocaConBomba())
    {
        dexter.setPosicion(dexter.getX(), anteriorY);
    }

    // Cuando Dexter termina de salir de su bomba, esta se vuelve solida
    // (la semilla del mono solo se revisa cuando ya cayo)
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        if (bombas[i]->estaDexterEncima() &&
            bombas[i]->getProgresoVuelo() >= 1 &&
            !tocaADexter(bombas[i]->getX(), bombas[i]->getY(),
                         bombas[i]->getAncho(), bombas[i]->getAlto()))
        {
            bombas[i]->dexterSeBajo();
        }
    }
}

bool Nivel2::dexterChocaConBomba()
{
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        if (bombas[i]->esSolidaParaDexter() &&
            tocaADexter(bombas[i]->getX(), bombas[i]->getY(),
                        bombas[i]->getAncho(), bombas[i]->getAlto()))
        {
            return true;
        }
    }
    return false;
}

// Hace explotar una bomba. Si era una semilla del mono,
// la inteligencia aprende si le pego a Dexter o no
void Nivel2::explotarBomba(int indice)
{
    Bomba *bomba = bombas[indice];
    bomba->explotar(mapa);

    if (bomba->esSemillaDelMono())
    {
        bool acerto = bomba->hayFuegoEn(getFilaDexter(), getColumnaDexter());
        inteligenciaMono.registrarResultado(bomba->getOpcionUsada(), acerto);
    }
}

void Nivel2::eliminarBomba(int indice)
{
    delete bombas[indice];
    bombas.erase(bombas.begin() + indice);
}

void Nivel2::eliminarTodasLasBombas()
{
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        delete bombas[i];
    }
    bombas.clear();
}

bool Nivel2::hayFuegoEnCasilla(int fila, int columna) const
{
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        if (bombas[i]->hayFuegoEn(fila, columna))
        {
            return true;
        }
    }
    return false;
}

int Nivel2::getFilaDexter()
{
    return (dexter.getY() + dexter.getAlto() / 2) / Mapa::TAM_CASILLA;
}

int Nivel2::getColumnaDexter()
{
    return (dexter.getX() + dexter.getAncho() / 2) / Mapa::TAM_CASILLA;
}

const std::vector<Bomba*> &Nivel2::getBombas() const
{
    return bombas;
}

//--------------------------------------------------- Enemigos ----------------------------------//

void Nivel2::crearEnemigos()
{
    eliminarTodosLosEnemigos();

    // Niebla(fila, columna, horizontal, casillas que recorre)
    nieblas.push_back(new Niebla(5, 4, true, 8));
    nieblas.push_back(new Niebla(2, 9, false, 8));

    // Armadillo(fila, columna)
    armadillos.push_back(new Armadillo(1, 11));

    // Mono(fila, columna)
    monos.push_back(new Mono(11, 2));

    // En dificil aparece una niebla y un armadillo mas
    if (dificil)
    {
        nieblas.push_back(new Niebla(9, 2, true, 10));
        armadillos.push_back(new Armadillo(7, 13));
    }

    // Cada mono arranca con una estrategia elegida por la inteligencia
    for (int i = 0; i < (int)monos.size(); i++)
    {
        monos[i]->setEstrategia(inteligenciaMono.elegirOpcion());
    }
}

void Nivel2::actualizarNieblas(float intervaloTiempo)
{
    int i = 0;

    while (i < (int)nieblas.size())
    {
        Niebla *niebla = nieblas[i];
        niebla->moverse(intervaloTiempo);

        // Casilla donde esta el centro de la niebla
        int fila = (niebla->getY() + niebla->getAlto() / 2) / Mapa::TAM_CASILLA;
        int columna = (niebla->getX() + niebla->getAncho() / 2) / Mapa::TAM_CASILLA;

        // Si el fuego de una bomba o un disparo la alcanza, desaparece
        if (hayFuegoEnCasilla(fila, columna) ||
            disparoAlcanza(niebla->getX(), niebla->getY(), niebla->getAncho(), niebla->getAlto()))
        {
            niebla->morir();
        }

        // Si toca a Dexter le quita una vida
        else if (tocaADexter(niebla->getX(), niebla->getY(),
                             niebla->getAncho(), niebla->getAlto()))
        {
            dexter.recibirDano();
        }

        // Borramos las nieblas que murieron
        if (!niebla->estaViva())
        {
            delete niebla;
            nieblas.erase(nieblas.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void Nivel2::actualizarArmadillos(float intervaloTiempo)
{
    int i = 0;

    while (i < (int)armadillos.size())
    {
        Armadillo *armadillo = armadillos[i];
        armadillo->actualizar(intervaloTiempo, mapa, bombas, getFilaDexter(), getColumnaDexter());

        int fila = (armadillo->getY() + armadillo->getAlto() / 2) / Mapa::TAM_CASILLA;
        int columna = (armadillo->getX() + armadillo->getAncho() / 2) / Mapa::TAM_CASILLA;

        // Muere si lo alcanza el fuego o un disparo
        if (hayFuegoEnCasilla(fila, columna) ||
            disparoAlcanza(armadillo->getX(), armadillo->getY(),
                           armadillo->getAncho(), armadillo->getAlto()))
        {
            armadillo->morir();
        }

        // Si golpea a Dexter le quita una vida
        // y si venia embistiendo, el golpe termina la embestida
        else if (tocaADexter(armadillo->getX(), armadillo->getY(),
                             armadillo->getAncho(), armadillo->getAlto()))
        {
            int vidasAntes = dexter.getVidas();
            dexter.recibirDano();

            // Si el golpe si le quito vida, lo empuja lejos del armadillo
            if (dexter.getVidas() < vidasAntes)
            {
                float distanciaX = (dexter.getX() + dexter.getAncho() / 2) -
                                   (armadillo->getX() + armadillo->getAncho() / 2);
                float distanciaY = (dexter.getY() + dexter.getAlto() / 2) -
                                   (armadillo->getY() + armadillo->getAlto() / 2);

                // Lo empuja en el eje donde estan mas separados
                const float FUERZA_GOLPE = 500;
                if (distanciaX * distanciaX >= distanciaY * distanciaY)
                {
                    if (distanciaX >= 0) dexter.aplicarImpulso(FUERZA_GOLPE, 0);
                    else dexter.aplicarImpulso(-FUERZA_GOLPE, 0);
                }
                else
                {
                    if (distanciaY >= 0) dexter.aplicarImpulso(0, FUERZA_GOLPE);
                    else dexter.aplicarImpulso(0, -FUERZA_GOLPE);
                }
            }

            if (armadillo->estaEmbistiendo())
            {
                armadillo->terminarEmbestida();
            }
        }

        // Si viene embistiendo y se lleva por delante al mono, tambien lo mata
        if (armadillo->estaVivo() && armadillo->estaEmbistiendo())
        {
            for (int m = 0; m < (int)monos.size(); m++)
            {
                Mono *mono = monos[m];

                if (mono->estaVivo() &&
                    seTocan(armadillo->getX(), armadillo->getY(), armadillo->getAncho(), armadillo->getAlto(),
                            mono->getX(), mono->getY(), mono->getAncho(), mono->getAlto()))
                {
                    mono->morir();
                    armadillo->terminarEmbestida();
                }
            }
        }

        if (!armadillo->estaVivo())
        {
            delete armadillo;
            armadillos.erase(armadillos.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void Nivel2::actualizarMonos(float intervaloTiempo)
{
    int i = 0;

    while (i < (int)monos.size())
    {
        Mono *mono = monos[i];
        mono->actualizar(intervaloTiempo, mapa, bombas, getFilaDexter(), getColumnaDexter());

        // Si el mono decidio lanzar, creamos la semilla
        if (mono->quiereLanzar())
        {
            bombas.push_back(new Bomba(mono->getFilaObjetivo(), mono->getColumnaObjetivo(),
                                       mono->getFila(), mono->getColumna(),
                                       mono->getEstrategia()));
            mono->confirmarLanzamiento();

            // Para el proximo ataque la inteligencia vuelve a escoger
            mono->setEstrategia(inteligenciaMono.elegirOpcion());
        }

        int fila = (mono->getY() + mono->getAlto() / 2) / Mapa::TAM_CASILLA;
        int columna = (mono->getX() + mono->getAncho() / 2) / Mapa::TAM_CASILLA;

        // Muere si lo alcanza el fuego o un disparo
        // (tocarlo no hace dano: el mono solo ataca con sus semillas)
        if (hayFuegoEnCasilla(fila, columna) ||
            disparoAlcanza(mono->getX(), mono->getY(), mono->getAncho(), mono->getAlto()))
        {
            mono->morir();
        }

        if (!mono->estaVivo())
        {
            delete mono;
            monos.erase(monos.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void Nivel2::eliminarTodosLosEnemigos()
{
    for (int i = 0; i < (int)nieblas.size(); i++)
    {
        delete nieblas[i];
    }
    nieblas.clear();

    for (int i = 0; i < (int)armadillos.size(); i++)
    {
        delete armadillos[i];
    }
    armadillos.clear();

    for (int i = 0; i < (int)monos.size(); i++)
    {
        delete monos[i];
    }
    monos.clear();
}

bool Nivel2::tocaADexter(float x, float y, int ancho, int alto)
{
    // Dos rectangulos se cruzan si se solapan en X y en Y a la vez
    return x < dexter.getX() + dexter.getAncho() &&
           dexter.getX() < x + ancho &&
           y < dexter.getY() + dexter.getAlto() &&
           dexter.getY() < y + alto;
}

void Nivel2::revisarPuerta()
{
    if (nieblas.empty() && armadillos.empty() && monos.empty() && !mapa.estaPuertaAbierta())
    {
        mapa.abrirPuerta();
    }
}

const std::vector<Niebla*> &Nivel2::getNieblas() const
{
    return nieblas;
}

const std::vector<Armadillo*> &Nivel2::getArmadillos() const
{
    return armadillos;
}

const std::vector<Mono*> &Nivel2::getMonos() const
{
    return monos;
}

//--------------------------------------------------- Consultas ----------------------------------//

Mapa &Nivel2::getMapa()
{
    return mapa;
}

Dexter &Nivel2::getDexter()
{
    return dexter;
}

bool Nivel2::haPerdido() const
{
    return dexter.getVidas() <= 0;
}

// Se gana al llegar a la puerta despues de abrirla
bool Nivel2::haGanado()
{
    return mapa.estaPuertaAbierta() &&
           mapa.getCasilla(getFilaDexter(), getColumnaDexter()) == Mapa::PUERTA;
}

//--------------------------------------------------- Power-ups ----------------------------------//

// Esconde los power-ups debajo de paredes rompibles escogidas al azar
void Nivel2::crearBeneficios()
{
    // Guardamos todas las casillas que tienen pared rompible
    std::vector<int> filasLibres;
    std::vector<int> columnasLibres;

    for (int f = 0; f < Mapa::FILAS; f++)
    {
        for (int c = 0; c < Mapa::COLUMNAS; c++)
        {
            if (mapa.getCasilla(f, c) == Mapa::PARED_ROMPIBLE)
            {
                filasLibres.push_back(f);
                columnasLibres.push_back(c);
            }
        }
    }

    // En dificil hay menos power-ups
    int cantidadSueros = 4;
    int cantidadPistolas = 3;
    if (dificil)
    {
        cantidadSueros = 3;
        cantidadPistolas = 2;
    }

    int total = cantidadSueros + cantidadPistolas;

    for (int i = 0; i < total && !filasLibres.empty(); i++)
    {
        // Escogemos una pared al azar y la quitamos de la lista
        // para no poner dos power-ups en la misma casilla
        int indice = rand() % filasLibres.size();

        int tipo = Beneficios::SUERO;
        if (i >= cantidadSueros)
        {
            tipo = Beneficios::PISTOLA;
        }

        beneficios.push_back(new Beneficios(filasLibres[indice], columnasLibres[indice], tipo));

        filasLibres.erase(filasLibres.begin() + indice);
        columnasLibres.erase(columnasLibres.begin() + indice);
    }
}

void Nivel2::actualizarBeneficios()
{
    int i = 0;

    while (i < (int)beneficios.size())
    {
        Beneficios *beneficio = beneficios[i];
        bool hayFuego = hayFuegoEnCasilla(beneficio->getFila(), beneficio->getColumna());

        // Si ya estaba a la vista y lo alcanza una explosion, se destruye
        if (!beneficio->estaOculto() && hayFuego)
        {
            delete beneficio;
            beneficios.erase(beneficios.begin() + i);
            continue;
        }

        // Solo se deja ver cuando se apaga el fuego que rompio la pared
        if (!hayFuego)
        {
            beneficio->actualizar(mapa);
        }

        // Dexter lo recoge al pasar por su casilla
        if (!beneficio->estaOculto() &&
            beneficio->getFila() == getFilaDexter() &&
            beneficio->getColumna() == getColumnaDexter())
        {
            if (beneficio->getTipo() == Beneficios::SUERO)
            {
                dexter.activarSueroNivel2();
            }
            else
            {
                dexter.activarPistola();
            }

            delete beneficio;
            beneficios.erase(beneficios.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

//--------------------------------------------------- Disparos ----------------------------------//

void Nivel2::disparar()
{
    if (!dexter.puedeDisparar())
    {
        return;
    }

    // El disparo sale del centro de Dexter hacia donde esta mirando
    float centroX = dexter.getX() + dexter.getAncho() / 2;
    float centroY = dexter.getY() + dexter.getAlto() / 2;

    disparos.push_back(new DisparoVeneno(centroX, centroY,
                                         dexter.getMirandoX(), dexter.getMirandoY()));
    dexter.registrarDisparo();
}

void Nivel2::actualizarDisparos(float intervaloTiempo)
{
    int i = 0;

    while (i < (int)disparos.size())
    {
        disparos[i]->actualizar(intervaloTiempo, mapa);

        // Borramos los que chocaron con una pared o con un enemigo
        if (!disparos[i]->estaActivo())
        {
            delete disparos[i];
            disparos.erase(disparos.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

// Revisa si algun disparo toca ese rectangulo (el de un enemigo).
// Si lo toca, el disparo se gasta
bool Nivel2::disparoAlcanza(float x, float y, int ancho, int alto)
{
    for (int i = 0; i < (int)disparos.size(); i++)
    {
        DisparoVeneno *disparo = disparos[i];

        if (disparo->estaActivo() &&
            x < disparo->getX() + disparo->getAncho() &&
            disparo->getX() < x + ancho &&
            y < disparo->getY() + disparo->getAlto() &&
            disparo->getY() < y + alto)
        {
            disparo->desactivar();
            return true;
        }
    }
    return false;
}

void Nivel2::eliminarBeneficiosYDisparos()
{
    for (int i = 0; i < (int)beneficios.size(); i++)
    {
        delete beneficios[i];
    }
    beneficios.clear();

    for (int i = 0; i < (int)disparos.size(); i++)
    {
        delete disparos[i];
    }
    disparos.clear();
}

const std::vector<Beneficios*> &Nivel2::getBeneficios() const
{
    return beneficios;
}

const std::vector<DisparoVeneno*> &Nivel2::getDisparos() const
{
    return disparos;
}

//--------------------------------------------------- Empujon ----------------------------------//

// Mueve a Dexter con el impulso del golpe. Si el empujon lo mete
// en una bomba solida, lo devolvemos y el empujon se acaba
void Nivel2::empujarDexter(float intervaloTiempo)
{
    float anteriorX = dexter.getX();
    float anteriorY = dexter.getY();

    dexter.moverPorImpulso(intervaloTiempo, mapa);

    if (dexterChocaConBomba())
    {
        dexter.setPosicion(anteriorX, anteriorY);
        dexter.detenerImpulso();
    }
}

// Dos rectangulos se cruzan si se solapan en X y en Y a la vez
bool Nivel2::seTocan(float x1, float y1, int ancho1, int alto1,
                     float x2, float y2, int ancho2, int alto2)
{
    return x1 < x2 + ancho2 && x2 < x1 + ancho1 &&
           y1 < y2 + alto2 && y2 < y1 + alto1;
}