#ifndef NIVEL1_H
#define NIVEL1_H

#include "dexter.h"

class Nivel1
{
private:
    Dexter dexter;

public:
    Nivel1();

    // Solicita a Dexter utilizar la red
    void recolectar();

    void actualizar(int direccion, float intervaloTiempo);
    void saltar();
    Dexter &getDexter();
};

#endif // NIVEL1_H