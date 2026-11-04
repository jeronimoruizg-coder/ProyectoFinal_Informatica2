#ifndef ENTIDAD_H
#define ENTIDAD_H

class Entidad
{
protected:
    float posicion_x;
    float posicion_y;
    int alto;
    int ancho;
    float velocidadMovimiento;

public:
    Entidad();
    virtual ~Entidad();

    void actualizarposicion(float posicionX, float posicionY);
    void obtenerHitbox();
    virtual void moverse();
};
#endif // ENTIDAD_H
