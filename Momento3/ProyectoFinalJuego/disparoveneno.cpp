#include "disparoveneno.h"

DisparoVeneno::DisparoVeneno(float centroX, float centroY, int direccionX, int direccionY)
{
    ancho = 12;
    alto = 12;

    // Sale desde el centro de Dexter
    x = centroX - ancho / 2;
    y = centroY - alto / 2;

    dirX = direccionX;
    dirY = direccionY;
    velocidad = 420;
    activo = true;
}

void DisparoVeneno::actualizar()
{
}

void DisparoVeneno::actualizar(float intervaloTiempo, Mapa &mapa)
{
    if (!activo)
    {
        return;
    }

    // MRU: avanza siempre a la misma velocidad
    x += dirX * velocidad * intervaloTiempo;
    y += dirY * velocidad * intervaloTiempo;

    // Si su centro entra en una pared, se acaba
    int fila = (y + alto / 2) / Mapa::TAM_CASILLA;
    int columna = (x + ancho / 2) / Mapa::TAM_CASILLA;

    if (mapa.esPared(fila, columna))
    {
        activo = false;
    }
}

bool DisparoVeneno::estaActivo() const { return activo; }
void DisparoVeneno::desactivar() { activo = false; }