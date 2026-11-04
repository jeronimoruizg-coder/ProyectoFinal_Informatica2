#ifndef DEXTER_H
#define DEXTER_H
#include "entidad.h"

class Dexter:public Entidad
{

public:
    Dexter();
    void atraparObjetos();
    void saltar();
    void despalzarse();

};

#endif // DEXTER_H
