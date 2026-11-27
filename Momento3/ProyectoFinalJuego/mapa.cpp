#include "mapa.h"

// 0 = pasillo, 1 = pared fija, 2 = pared rompible, 3 = puerta
const int Mapa::DISENO[Mapa::FILAS][Mapa::COLUMNAS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,2,2,2,0,2,2,2,2,0,2,2,1},
    {1,0,1,2,1,2,2,0,1,0,1,2,1,2,1},
    {1,2,2,2,2,0,2,0,2,0,2,2,2,2,1},
    {1,2,1,2,2,0,1,2,1,2,2,2,1,2,1},
    {1,2,2,0,0,0,0,0,2,2,2,2,2,0,1},
    {1,2,1,2,1,2,2,2,1,2,2,2,1,0,3},
    {1,2,2,2,2,2,2,0,2,2,2,0,0,0,1},
    {1,2,1,2,2,2,1,2,2,2,1,0,1,2,1},
    {1,0,0,2,2,2,2,2,2,0,0,0,0,2,1},
    {1,2,1,2,1,2,2,2,1,2,1,0,1,0,1},
    {1,2,2,2,2,2,2,2,2,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

Mapa::Mapa()
{
    reiniciar();
}

void Mapa::reiniciar()
{
    puertaAbierta = false;

    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            casillas[f][c] = DISENO[f][c];
        }
    }
}

int Mapa::getCasilla(int fila, int columna)
{
    // lo que quede por fuera del mapa cuenta como pared
    if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
        return PARED_FIJA;
    }
    return casillas[fila][columna];
}

bool Mapa::esPared(int fila, int columna)
{
    int tipo = getCasilla(fila, columna);

    if (tipo == PUERTA) {
        return !puertaAbierta;
    }
    return tipo == PARED_FIJA || tipo == PARED_ROMPIBLE;
}

void Mapa::romperPared(int fila, int columna)
{
    if (getCasilla(fila, columna) == PARED_ROMPIBLE) {
        casillas[fila][columna] = PASILLO;
    }
}

void Mapa::abrirPuerta()
{
    puertaAbierta = true;
}

bool Mapa::estaPuertaAbierta()
{
    return puertaAbierta;
}