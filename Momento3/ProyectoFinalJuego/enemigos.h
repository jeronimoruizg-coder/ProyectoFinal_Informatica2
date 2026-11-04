#ifndef ENEMIGOS_H
#define ENEMIGOS_H
#include "entidad.h"

class Enemigos: public Entidad
{
public:
    Enemigos();
    void moverse() override;
};

#endif // ENEMIGOS_H
