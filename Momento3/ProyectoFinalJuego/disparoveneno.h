#ifndef DISPAROVENENO_H
#define DISPAROVENENO_H

#include "objetos.h"
#include "mapa.h"

// Disparo de la pistola de veneno. Avanza en linea recta (MRU)
// hasta chocar con una pared o con un enemigo.
class DisparoVeneno : public Objetos
{
private:
    int dirX;
    int dirY;
    float velocidad;
    bool activo;

public:
    DisparoVeneno(float centroX, float centroY, int direccionX, int direccionY);

    void actualizar() override;
    void actualizar(float intervaloTiempo, Mapa &mapa);

    bool estaActivo() const;
    void desactivar();
};

#endif // DISPAROVENENO_H