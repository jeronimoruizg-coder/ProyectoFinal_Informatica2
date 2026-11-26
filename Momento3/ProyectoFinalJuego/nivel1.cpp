#include "nivel1.h"

Nivel1::Nivel1()
{
    dexter.iniciarNivel1();
}

void Nivel1::actualizar(int direccion, float intervaloTiempo)
{
    dexter.desplazarse(direccion, intervaloTiempo);
    dexter.aplicarGravedad(intervaloTiempo);
    // Actualizamos el tiempo de uso de la red
    dexter.actualizarRecoleccion(intervaloTiempo);
}

void Nivel1::saltar()
{
    dexter.saltar();
}

Dexter &Nivel1::getDexter()
{
    return dexter;
}

// Solicitamos a Dexter iniciar una recoleccion
void Nivel1::recolectar()
{
    dexter.atraparObjetos();
}