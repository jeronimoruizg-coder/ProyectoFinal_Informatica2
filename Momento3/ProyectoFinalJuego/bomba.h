#ifndef BOMBA_H
#define BOMBA_H

#include "objetos.h"

class Bomba : public Objetos
{
public:
    Bomba();
    void actualizar() override;
};

#endif // BOMBA_H