#include "enemigos.h"

Enemigos::Enemigos()
{
    x = 0;
    y = 0;

    ancho = 0;
    alto = 0;

    velocidad = 0;
    direccion = 1;
}

Enemigos::~Enemigos()
{
}

void Enemigos::moverse()
{
    moverse(0.016);
}

// Movimiento rectilineo uniforme
void Enemigos::moverse(float intervaloTiempo)
{
    x += direccion * velocidad * intervaloTiempo;
}

float Enemigos::getX() const
{
    return x;
}

float Enemigos::getY() const
{
    return y;
}

int Enemigos::getAncho() const
{
    return ancho;
}

int Enemigos::getAlto() const
{
    return alto;
}

int Enemigos::getDireccion() const
{
    return direccion;
}