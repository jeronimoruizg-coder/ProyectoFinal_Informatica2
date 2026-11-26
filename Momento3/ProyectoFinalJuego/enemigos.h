#ifndef ENEMIGOS_H
#define ENEMIGOS_H

class Enemigos
{
protected:

    // Posicion del enemigo
    float x, y;

    // Dimensiones fisicas
    int ancho, alto;

    // Velocidad de movimiento
    float velocidad;

    // 1: derecha, -1: izquierda
    int direccion;

public:

    Enemigos();
    virtual ~Enemigos();

    virtual void moverse();

    // Movimiento utilizando el tiempo transcurrido
    virtual void moverse(float intervaloTiempo);

    // Consultas de posicion y dimensiones
    float getX() const;
    float getY() const;

    int getAncho() const;
    int getAlto() const;
    int getDireccion() const;
};

#endif // ENEMIGOS_H
