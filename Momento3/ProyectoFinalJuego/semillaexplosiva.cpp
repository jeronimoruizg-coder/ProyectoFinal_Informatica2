#include "semillaexplosiva.h"

// Constructor de la semilla explosiva
SemillaExplosiva::SemillaExplosiva(float posicionX)
{
    // Dimensiones fisicas
    ancho = 40;
    alto = 40;

    // Posicion inicial
    x = posicionX;
    y = -alto;

    // Comienza cayendo desde el reposo
    velocidadY = 0;

    // Inicialmente no ha explotado
    explotada = false;

    tiempoExplosion = 0;
}

// Funcion heredada de Objetos
void SemillaExplosiva::actualizar()
{
    actualizar(0.016);
}

// Actualizamos la fisica de la semilla
void SemillaExplosiva::actualizar(float intervaloTiempo)
{
    // Si exploto, avanzamos el tiempo de la animacion
    if (explotada)
    {
        tiempoExplosion += intervaloTiempo;
        return;
    }

    // Actualizamos la velocidad vertical
    velocidadY += 180 * intervaloTiempo;

    // Actualizamos la posicion vertical
    y += velocidadY * intervaloTiempo;
}

// Verificamos si la semilla alcanzo el suelo
bool SemillaExplosiva::llegoAlSuelo() const
{
    return y + alto >= 650;
}

// Activamos la explosion
void SemillaExplosiva::explotar()
{
    explotada = true;
    tiempoExplosion = 0;
}

// Consultamos si la semilla ha explotado
bool SemillaExplosiva::haExplotado() const
{
    return explotada;
}

// Calculamos el fotograma actual de la explosion
int SemillaExplosiva::getFotogramaExplosion() const
{
    int fotograma = int(tiempoExplosion / 0.08);

    // Evitamos acceder a un fotograma inexistente
    if (fotograma > 7)
    {
        fotograma = 7;
    }

    return fotograma;
}

// Verificamos si termino la animacion
bool SemillaExplosiva::terminoExplosion() const
{
    return explotada && tiempoExplosion >= 0.64;
}