#ifndef OBJETOS_H
#define OBJETOS_H

class Objetos
{
public:
    Objetos();
    virtual ~Objetos() {}
    virtual void actualizar() = 0;
};

#endif // OBJETOS_H