#ifndef NIVEL2_H
#define NIVEL2_H

#include "mapa.h"
#include "dexter.h"

class Nivel2
{
private:
    Mapa mapa;
    Dexter dexter;
    bool dificil;

public:
    Nivel2(bool dificultad = false);

    // Deja el nivel como al inicio
    void reiniciar();

    // Avanza el juego un intervalo de tiempo
    void actualizar(int dirX, int dirY, float intervaloTiempo);

    Mapa &getMapa();
    Dexter &getDexter();

    bool haPerdido() const;
};

#endif // NIVEL2_H