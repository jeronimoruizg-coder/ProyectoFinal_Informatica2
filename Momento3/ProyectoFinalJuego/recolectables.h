#ifndef RECOLECTABLES_H
#define RECOLECTABLES_H

#include "objetos.h"

class Recolectables : public Objetos
{
private:
    float velocidadY;
    float posicionInicialX;
    float tiempoCaida;

    bool especial;
    bool dificil;

    // Identifica la apariencia de la Flor recolectable comun
    int variante;//permitirá que cada muestra conserve su apariencia durante toda la caída.
public:
    Recolectables(float posicionX, bool esEspecial,
                  bool dificultad);

    // Actualizacion del movimiento del objeto
    void actualizar();
    void actualizar(float intervaloTiempo);

    // Comprueba si la muestra llego al suelo
    bool llegoAlSuelo();

    // Indica cuantos puntos aporta la muestra
    int sumarPuntos();

    // Identifica el tipo de muestra
    bool esEspecial();

    // Devuelve la variante grafica de la Flor
    int getVariante();
};

#endif // RECOLECTABLES_H