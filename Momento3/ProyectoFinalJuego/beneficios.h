#ifndef BENEFICIOS_H
#define BENEFICIOS_H

#include "objetos.h"
#include "mapa.h"

class Beneficios : public Objetos
{
private:

    // Velocidad vertical durante la caida
    float velocidadY;

    // 0: Planta medicinal
    // 1: Suero de velocidad
    int tipo;

public:

    Beneficios(float posicionX, int tipoBeneficio);

    void actualizar() override;
    void actualizar(float intervaloTiempo);

    bool llegoAlSuelo() const;

    int getTipo() const;


    //------------------------------------------------ nivel 2 ------------------------------------//
    // En el nivel 2 los beneficios no caen: estan escondidos
    // debajo de una pared rompible y aparecen cuando la rompen
    static const int SUERO = 1;
    static const int PISTOLA = 2;

    Beneficios(int filaInicial, int columnaInicial, int tipoBeneficio);

    // Revisa si ya rompieron la pared que lo tapaba
    void actualizar(Mapa &mapa);

    bool estaOculto() const;
    int getFila() const;
    int getColumna() const;

private:
    // Atributos del nivel 2
    int fila;
    int columna;
    bool oculto;
};

#endif // BENEFICIOS_H