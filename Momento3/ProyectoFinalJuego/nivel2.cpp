#include "nivel2.h"

Nivel2::Nivel2(bool dificultad)
{
    dificil = dificultad;
    reiniciar();
}

void Nivel2::reiniciar()
{
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
}

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