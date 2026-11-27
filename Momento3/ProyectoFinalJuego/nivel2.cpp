#include "nivel2.h"

Nivel2::Nivel2(bool dificultad)
{
    dificil = dificultad;
    reiniciar();
}

Nivel2::~Nivel2()
{
    // Liberamos las bombas que queden al cerrar el nivel
    eliminarTodasLasBombas();
}

void Nivel2::reiniciar()
{
    eliminarTodasLasBombas();
    mapa.reiniciar();
    dexter.iniciarNivel2();
}

void Nivel2::actualizar(int dirX, int dirY, float intervaloTiempo)
{
    // Si la partida termino no actualizamos nada
    if (haPerdido())
    {
        return;
    }

    dexter.mover(dirX, dirY, intervaloTiempo, mapa);
    dexter.actualizarInvulnerabilidad(intervaloTiempo);

    actualizarBombas(intervaloTiempo);
}

//--------------------------------------------------- Bombas ----------------------------------//

void Nivel2::ponerBomba()
{
    int fila = getFilaDexter();
    int columna = getColumnaDexter();

    // No dejamos poner dos bombas en la misma casilla
    for (int i = 0; i < (int)bombas.size(); i++)
    {
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