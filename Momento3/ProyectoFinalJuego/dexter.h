#ifndef DEXTER_H
#define DEXTER_H

#include "mapa.h"

class Dexter
{
private:
    float x, y;
    int ancho, alto;
    float velocidad;

    bool chocaConPared(float nuevaX, float nuevaY, Mapa &mapa);

public:
    Dexter();

    void mover(int dirX, int dirY, Mapa &mapa);

    float getX();
    float getY();
    int getAncho();
    int getAlto();

    // nivel 1
    void atraparObjetos();
    void saltar();
    void despalzarse();
};

#endif // DEXTER_H