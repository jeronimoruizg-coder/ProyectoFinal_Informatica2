#ifndef JABALI_H
#define JABALI_H

#include "enemigos.h"

class Jabali : public Enemigos
{
public:

    Jabali(int sentido);

    bool salioDelEscenario() const;
};

#endif // JABALI_H
