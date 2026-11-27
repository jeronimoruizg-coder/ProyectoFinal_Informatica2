#include "niebla.h"
#include "mapa.h"

Niebla::Niebla(int fila, int columna, bool movimientoHorizontal, int casillasRecorrido)
{
    ancho = 40;
    alto = 40;
    velocidad = 70;   // lenta, es una niebla
    direccion = 1;
    viva = true;

    horizontal = movimientoHorizontal;

    // Arranca centrada en su casilla
    int tam = Mapa::TAM_CASILLA;
    x = columna * tam + (tam - ancho) / 2;
    y = fila * tam + (tam - alto) / 2;

    // El recorrido va desde la casilla inicial hasta "casillasRecorrido" mas adelante
    if (horizontal)
    {
        limiteInicio = x;
        limiteFin = x + casillasRecorrido * tam;
    }
    else
    {
        limiteInicio = y;
        limiteFin = y + casillasRecorrido * tam;
    }
}

// Movimiento rectilineo uniforme, ignorando las paredes
void Niebla::moverse(float intervaloTiempo)
{
    float paso = direccion * velocidad * intervaloTiempo;

    if (horizontal)
    {
        x += paso;

        // Al llegar a un extremo se devuelve
        if (x >= limiteFin)
        {
            x = limiteFin;
            direccion = -1;
        }
        else if (x <= limiteInicio)
        {
            x = limiteInicio;
            direccion = 1;
        }
    }
    else
    {
        y += paso;

        if (y >= limiteFin)
        {
            y = limiteFin;
            direccion = -1;
        }
        else if (y <= limiteInicio)
        {
            y = limiteInicio;
            direccion = 1;
        }
    }
}

void Niebla::morir()
{
    viva = false;
}

bool Niebla::estaViva() const
{
    return viva;
}