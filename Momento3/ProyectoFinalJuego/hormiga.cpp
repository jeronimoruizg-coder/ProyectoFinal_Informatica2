#include "hormiga.h"

Hormiga::Hormiga(int sentido)
{
    // Dimensiones fisicas de la hormiga
    ancho = 70;
    alto = 50;

    velocidad = 120;

    direccion = sentido;

    // Ubicamos la hormiga sobre el suelo
    y = 650 - alto;

    // Determinamos desde que extremo aparece
    if (direccion == 1)
    {
        x = -ancho;
    }
    else
    {
        x = 1280;
    }
}

// Comprobamos si salio completamente del escenario
bool Hormiga::salioDelEscenario() const
{
    if (direccion == 1)
    {
        return x >= 1280;
    }

    return x + ancho <= 0;
}