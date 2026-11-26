#ifndef HORMIGA_H
#define HORMIGA_H

#include "enemigos.h"

class Hormiga : public Enemigos
{
public:

    Hormiga(int sentido);

    bool salioDelEscenario() const;
};

#endif // HORMIGA_H