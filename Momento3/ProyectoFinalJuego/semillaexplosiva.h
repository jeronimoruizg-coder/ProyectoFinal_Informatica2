#ifndef SEMILLAEXPLOSIVA_H
#define SEMILLAEXPLOSIVA_H

#include "objetos.h"

class SemillaExplosiva : public Objetos
{
private:

    // Velocidad vertical durante la caida
    float velocidadY;

    // Indica si la semilla ha explotado
    bool explotada;

    // Tiempo transcurrido desde que exploto
    float tiempoExplosion;
public:

    SemillaExplosiva(float posicionX = 0);

    void actualizar() override;
    void actualizar(float intervaloTiempo);

    bool llegoAlSuelo() const;

    void explotar();
    bool haExplotado() const;

    // Consultamos el fotograma de la explosion
    int getFotogramaExplosion() const;

    // Verificamos si termino la explosion
    bool terminoExplosion() const;
};

#endif // SEMILLAEXPLOSIVA_H