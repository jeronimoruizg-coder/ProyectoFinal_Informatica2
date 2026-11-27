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

    // Sistema de vidas y daño
    int vidas;
    float tiempoInvulnerabilidad;

    // Tiempo restante del efecto del suero
    float tiempoSuero;

    // Tiempo restante de ralentizacion
    float tiempoRalentizacion;
public:
    Dexter();

    void mover(int dirX, int dirY, float intervaloTiempo, Mapa &mapa);

    float getX();
    float getY();
    int getAncho();
    int getAlto();
 //------------------------------------------------ nivel 1 ------------------------------------//
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

    // Sistema de daño
    void recibirDano();
    void actualizarInvulnerabilidad(float intervaloTiempo);

    // Consultas del estado de Dexter
    int getVidas() const;
    bool esInvulnerable() const;
    bool debeTitilarRojo() const;

    // Beneficios para Dexter
    void recuperarVida();
    void activarSuero();
    void actualizarSuero(float intervaloTiempo);
    bool tieneSuero() const;

    //Efecto de excremento
    void ralentizar();
    void actualizarRalentizacion(float intervaloTiempo);
    bool estaRalentizado() const;

//------------------------------------------------ nivel 2 ------------------------------------//
    void iniciarNivel2();
    void setPosicion(float nuevaX, float nuevaY);


    // Power-ups del nivel 2
    void activarSueroNivel2();
    void activarPistola();
    void actualizarPoderes(float intervaloTiempo);
    bool tienePistola() const;
    bool puedeDisparar() const;
    void registrarDisparo();
    float getTiempoSuero() const;
    float getTiempoPistola() const;

    // Hacia donde mira Dexter (para saber hacia donde dispara)
    int getMirandoX() const;
    int getMirandoY() const;

    // Empujon del armadillo (impulso con amortiguamiento)
    void aplicarImpulso(float impulsoHorizontal, float impulsoVertical);
    void moverPorImpulso(float intervaloTiempo, Mapa &mapa);
    void detenerImpulso();

private:
    // Atributos del nivel 2
    float tiempoPistola;
    float recargaDisparo;
    int mirandoX;
    int mirandoY;

    // Velocidad del empujon (pixeles por segundo)
    float impulsoX;
    float impulsoY;
};

#endif // DEXTER_H