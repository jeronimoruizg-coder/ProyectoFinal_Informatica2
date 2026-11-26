#ifndef GUACAMAYA_H
#define GUACAMAYA_H

#include "enemigos.h"

class Guacamaya : public Enemigos
{
private:

    // Indica si ya lanzo un excremento
    bool excrementoLanzado;
    // Estrategia de prediccion seleccionada
    int estrategia;

public:

    Guacamaya(int sentido);

    bool salioDelEscenario() const;

    bool haLanzadoExcremento() const;
    void registrarLanzamiento();

    void asignarEstrategia(int opcion);
    int getEstrategia() const;
};

#endif // GUACAMAYA_H
