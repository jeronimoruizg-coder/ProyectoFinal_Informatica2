#include "excremento.h"

Excremento::Excremento(float posicionX, float posicionY, int opcion)
{
    ancho = 25;
    alto = 30;

    x = posicionX;
    y = posicionY;

    velocidadY = 0;

    enSuelo = false;
    tiempoEnSuelo = 0;

    estrategia = opcion;
}

void Excremento::actualizar()
{
    actualizar(0.016);
}

void Excremento::actualizar(float intervaloTiempo)
{
    // Si ya esta en el suelo, acumulamos el tiempo
    if (enSuelo)
    {
        tiempoEnSuelo += intervaloTiempo;
        return;
    }

    // Movimiento vertical acelerado
    velocidadY += 450 * intervaloTiempo;

    y += velocidadY * intervaloTiempo;

    // Detectamos el contacto con el suelo
    if (y + alto >= 650)
    {
        enSuelo = true;

        velocidadY = 0;

        // Dimensiones del excremento aplastado
        x -= 10;
        ancho = 45;
        alto = 15;

        y = 650 - alto;
    }
}

bool Excremento::estaEnSuelo() const
{
    return enSuelo;
}

bool Excremento::debeDesaparecer() const
{
    return tiempoEnSuelo >= 2;
}


int Excremento::getEstrategia() const
{
    return estrategia;
}