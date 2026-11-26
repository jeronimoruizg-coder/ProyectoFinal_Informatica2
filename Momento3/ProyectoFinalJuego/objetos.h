#ifndef OBJETOS_H
#define OBJETOS_H

class Objetos
{
protected:
    // Posicion del objeto en el escenario
    float x;
    float y;

    // Dimensiones fisicas del objeto
    int ancho;
    int alto;

public:
    Objetos();

    // Destructor virtual para las clases derivadas
    virtual ~Objetos() {}

    // Metodo abstracto que cada clase hija implementa
    // segun su propio comportamiento
    virtual void actualizar() = 0;

    // Consultamos la posicion del objeto
    float getX();
    float getY();

    // Consultamos las dimensiones del objeto
    int getAncho();
    int getAlto();
};

#endif // OBJETOS_H