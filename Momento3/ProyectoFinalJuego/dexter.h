#ifndef DEXTER_H
#define DEXTER_H

#include "mapa.h"

class Dexter
{
private:
    float x, y;
    int ancho, alto;
    float velocidad;

    bool chocaConPared(float nuevaX, float nuevaY, Mapa &mapa);

    //Adiciones para nivel 1
    float velocidadY;
    // Tiempo restante de la accion de recoleccion
    float tiempoRecoleccion;
    // Orientacion: 1 derecha, -1 izquierda
    int orientacion;
public:
    Dexter();
    //nivel 2
    void mover(int dirX, int dirY, Mapa &mapa);

    float getX();
    float getY();
    int getAncho();
    int getAlto();

    // nivel 1
    void iniciarNivel1();
    void desplazarse(int direccion, float intervaloTiempo);
    void saltar();
    void aplicarGravedad(float intervaloTiempo);
    void atraparObjetos();
    // Actualiza el tiempo restante de recoleccion
    void actualizarRecoleccion(float intervaloTiempo);

    // Indica si Dexter esta utilizando la red
    bool estaRecolectando();

    // Devuelve la orientacion del personaje
    int getOrientacion();

    // Coordenadas de la zona de captura
    float getRecoleccionX();
    float getRecoleccionY();

    // Dimensiones de la zona de captura
    int getRecoleccionAncho();
    int getRecoleccionAlto();
};

#endif // DEXTER_H