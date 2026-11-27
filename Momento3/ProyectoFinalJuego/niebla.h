#ifndef NIEBLA_H
#define NIEBLA_H

#include "enemigos.h"

// Enemigo del nivel 2 que atraviesa las paredes.
// Va y viene en linea recta entre dos casillas (movimiento predecible)
class Niebla : public Enemigos
{
private:
    // true: se mueve en horizontal, false: en vertical
    bool horizontal;

    // Limites del recorrido en pixeles
    float limiteInicio;
    float limiteFin;

    bool viva;

public:
    Niebla(int fila, int columna, bool movimientoHorizontal, int casillasRecorrido);

    void moverse(float intervaloTiempo) override;

    void morir();
    bool estaViva() const;
};

#endif // NIEBLA_H