#include "beneficios.h"

// Constructor del objeto beneficioso
Beneficios::Beneficios(float posicionX, int tipoBeneficio)
{
    ancho = 40;
    alto = 40;

    x = posicionX;
    y = -alto;

    velocidadY = 0;

    tipo = tipoBeneficio;
}

// Funcion heredada de Objetos
void Beneficios::actualizar()
{
    actualizar(0.016);
}

// Actualizamos la fisica de caida
void Beneficios::actualizar(float intervaloTiempo)
{
    velocidadY += 180 * intervaloTiempo;

    y += velocidadY * intervaloTiempo;
}

// Comprobamos si alcanzo el suelo
bool Beneficios::llegoAlSuelo() const
{
    return y + alto >= 650;
}

// Consultamos el tipo de beneficio
int Beneficios::getTipo() const
{
    return tipo;
}