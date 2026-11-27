#include "nivel2.h"
#include <cstdlib>
#include <ctime>

Nivel2::Nivel2(bool dificultad)
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
}

void Nivel2::reiniciar()
{
    eliminarTodasLasBombas();
    mapa.reiniciar();
    dexter.iniciarNivel2();
    crearEnemigos();
}

void Nivel2::actualizar(int dirX, int dirY, float intervaloTiempo)
{
    // Si la partida termino no actualizamos nada
    if (haPerdido() || haGanado())
    {
        return;
    }

    dexter.mover(dirX, dirY, intervaloTiempo, mapa);
    dexter.actualizarInvulnerabilidad(intervaloTiempo);

    actualizarBombas(intervaloTiempo);
    actualizarNieblas(intervaloTiempo);
    actualizarArmadillos(intervaloTiempo);
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
        if (!bombas[i]->haExplotado())
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
            bombas[i]->explotar(mapa);
        }
    }

    // Reaccion en cadena: el fuego hace explotar otras bombas
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        if (!bombas[i]->haExplotado() &&
            hayFuegoEnCasilla(bombas[i]->getFila(), bombas[i]->getColumna()))
        {
            bombas[i]->explotar(mapa);
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
    armadillos.push_back(new Armadillo(11, 13));

    // En dificil aparece una niebla y un armadillo mas
    if (dificil)
    {
        nieblas.push_back(new Niebla(9, 2, true, 10));
        armadillos.push_back(new Armadillo(7, 13));
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

        // Si el fuego de una bomba la alcanza, desaparece
        if (hayFuegoEnCasilla(fila, columna))
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
        armadillo->actualizar(intervaloTiempo, mapa, getFilaDexter(), getColumnaDexter());

        int fila = (armadillo->getY() + armadillo->getAlto() / 2) / Mapa::TAM_CASILLA;
        int columna = (armadillo->getX() + armadillo->getAncho() / 2) / Mapa::TAM_CASILLA;

        // Muere si lo alcanza el fuego
        if (hayFuegoEnCasilla(fila, columna))
        {
            armadillo->morir();
        }

        // Si golpea a Dexter le quita una vida
        // y si venia embistiendo, el golpe termina la embestida
        else if (tocaADexter(armadillo->getX(), armadillo->getY(),
                             armadillo->getAncho(), armadillo->getAlto()))
        {
            dexter.recibirDano();

            if (armadillo->estaEmbistiendo())
            {
                armadillo->terminarEmbestida();
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
    if (nieblas.empty() && armadillos.empty() && !mapa.estaPuertaAbierta())
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