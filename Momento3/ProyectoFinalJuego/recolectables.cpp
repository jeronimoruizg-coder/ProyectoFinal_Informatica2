#include "recolectables.h"
#include <cmath>
#include <cstdlib>//La utilizamos para generar un numero aleatorio con rand()

using namespace std;

Recolectables::Recolectables(float posicionX,
                             bool esEspecial,
                             bool dificultad)
{
    // Dimensiones fisicas de la muestra
    ancho = 40;
    alto = 40;

    // Posicion inicial
    x = posicionX;
    y = -alto;

    posicionInicialX = posicionX;

    velocidadY = 0;
    tiempoCaida = 0;

    especial = esEspecial;
    dificil = dificultad;

    // Seleccionamos una de las cuatro flores comunes
    // La apariencia se conserva durante toda la caida
    variante = 0;

    if (!especial)
    {
        variante = rand() % 4;
    }
}

// Implementamos el metodo virtual de Objetos
void Recolectables::actualizar()
{
    actualizar(0.016);
}

// Actualizamos el movimiento de la muestra
void Recolectables::actualizar(float intervaloTiempo)
{
    tiempoCaida += intervaloTiempo;

    // Movimiento vertical con aceleracion (MRUA)
    velocidadY += 180 * intervaloTiempo;
    y += velocidadY * intervaloTiempo;

    // Movimiento oscilatorio en dificultad dificil
    if (dificil)
    {
        x = posicionInicialX + 45 * sin(4 * tiempoCaida);
    }

    // Limite izquierdo
    if (x < 0)
    {
        x = 0;
    }

    // Limite derecho
    if (x + ancho > 1280)
    {
        x = 1280 - ancho;
    }
}

// Detectamos si la muestra toca el suelo
bool Recolectables::llegoAlSuelo()
{
    return y + alto >= 650;
}

// Determinamos el valor de la muestra
int Recolectables::sumarPuntos()
{
    if (especial && !dificil)
    {
        return 2;
    }

    return 1;
}

// Consultamos si la muestra es especial
bool Recolectables::esEspecial()
{
    return especial;
}

// Consultamos la variante grafica de la muestra
int Recolectables::getVariante()
{
    return variante;
}