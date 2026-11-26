#ifndef EXCREMENTO_H
#define EXCREMENTO_H

#include "objetos.h"

class Excremento : public Objetos
{
private:

    float velocidadY;

    // Indica si alcanzo el suelo
    bool enSuelo;

    // Tiempo acumulado sobre el suelo
    float tiempoEnSuelo;

    // Estrategia que produjo el lanzamiento
    int estrategia;

public:

    Excremento(float posicionX, float posicionY, int opcion = -1);

    void actualizar() override;
    void actualizar(float intervaloTiempo);

    bool estaEnSuelo() const;
    bool debeDesaparecer() const;

    int getEstrategia() const;
};

#endif // EXCREMENTO_H
