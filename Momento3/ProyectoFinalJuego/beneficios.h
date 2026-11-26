#ifndef BENEFICIOS_H
#define BENEFICIOS_H

#include "objetos.h"

class Beneficios : public Objetos
{
private:

    // Velocidad vertical durante la caida
    float velocidadY;

    // 0: Planta medicinal
    // 1: Suero de velocidad
    int tipo;

public:

    Beneficios(float posicionX, int tipoBeneficio);

    void actualizar() override;
    void actualizar(float intervaloTiempo);

    bool llegoAlSuelo() const;

    int getTipo() const;
};

#endif // BENEFICIOS_H