#include "dexter.h"

Dexter::Dexter()
{
    ancho = 36;
    alto = 36;
    velocidad = 3;

    // arranca centrado en la casilla (1, 1)
    x = 1 * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    y = 1 * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;
}

void Dexter::mover(int dirX, int dirY, Mapa &mapa)
{
    // cada eje se revisa por separado para que pueda deslizarse por las paredes
    float nuevaX = x + dirX * velocidad;
    if (!chocaConPared(nuevaX, y, mapa)) {
        x = nuevaX;
    }

    float nuevaY = y + dirY * velocidad;
    if (!chocaConPared(x, nuevaY, mapa)) {
        y = nuevaY;
    }
}

bool Dexter::chocaConPared(float nuevaX, float nuevaY, Mapa &mapa)
{
    int tam = Mapa::TAM_CASILLA;

    // casillas donde caerian las 4 esquinas de Dexter
    int columnaIzq = nuevaX / tam;
    int columnaDer = (nuevaX + ancho - 1) / tam;
    int filaArriba = nuevaY / tam;
    int filaAbajo = (nuevaY + alto - 1) / tam;

    return mapa.esPared(filaArriba, columnaIzq) || mapa.esPared(filaArriba, columnaDer)
           || mapa.esPared(filaAbajo, columnaIzq) || mapa.esPared(filaAbajo, columnaDer);
}

float Dexter::getX() { return x; }
float Dexter::getY() { return y; }
int Dexter::getAncho() { return ancho; }
int Dexter::getAlto() { return alto; }

void Dexter::atraparObjetos() {}
void Dexter::saltar() {}
void Dexter::despalzarse() {}