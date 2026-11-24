#ifndef INTELIGENCIA_H
#define INTELIGENCIA_H

class Inteligencia
{
public:
    Inteligencia(int numOpciones);
    int elegirOpcion();
    void registrarResultado(bool acerto);
};

#endif // INTELIGENCIA_H