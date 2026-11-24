#ifndef SEMILLAEXPLOSIVA_H
#define SEMILLAEXPLOSIVA_H

#include "bomba.h"

class SemillaExplosiva : public Bomba
{
public:
    SemillaExplosiva();
    void actualizar() override;
};

#endif // SEMILLAEXPLOSIVA_H