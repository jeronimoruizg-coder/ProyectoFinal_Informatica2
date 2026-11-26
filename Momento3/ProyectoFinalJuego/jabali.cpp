#include "jabali.h"

Jabali::Jabali(int sentido)
{
    // Dimensiones fisicas
    ancho = 110;
    alto = 60;

    // Velocidad de desplazamiento
    velocidad = 750;

    direccion = sentido;

    // Ubicamos al jabali sobre el suelo
    y = 650 - alto;

    // Determinamos desde donde aparece
    if (direccion == 1)
    {
        x = -ancho;
    }
    else
    {
        x = 1280;
    }
}

// Comprobamos si abandono completamente el escenario
bool Jabali::salioDelEscenario() const
{
    if (direccion == 1)
    {
        return x >= 1280;
    }

    return x + ancho <= 0;
}
