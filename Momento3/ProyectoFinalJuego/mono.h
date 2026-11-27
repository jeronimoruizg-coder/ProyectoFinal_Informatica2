#ifndef MONO_H
#define MONO_H

#include "enemigos.h"
#include "mapa.h"
#include "bomba.h"
#include <vector>

// Agente inteligente del nivel 2.
// Percibe a Dexter y las bombas, decide que hacer, actua
// (se mueve o lanza semillas) y su forma de atacar la elige
// la Inteligencia que tiene el nivel.
class Mono : public Enemigos
{
private:
    // Casilla donde esta y casilla a la que va
    int fila, columna;
    int filaDestino, columnaDestino;

    // Direccion en la que venia caminando (para pasear)
    int dirFila, dirColumna;

    bool vivo;

    // Segundos que faltan para poder lanzar otra semilla
    float tiempoRecarga;

    // Estrategia de ataque actual (la escoge la Inteligencia)
    int estrategia;

    // Lanzamiento pendiente: el nivel crea la semilla
    bool lanzamientoPendiente;
    int filaObjetivo, columnaObjetivo;

    // Para saber si Dexter se esta acercando
    int distanciaAnteriorDexter;

    // ---- Percibir ----
    bool hayBombaEn(const std::vector<Bomba*> &bombas, int f, int c);
    bool estaLibre(Mapa &mapa, const std::vector<Bomba*> &bombas, int f, int c);
    bool estaEnPeligro(Mapa &mapa, const std::vector<Bomba*> &bombas, int f, int c);

    // ---- Razonar y actuar ----
    bool tieneSemillaActiva(const std::vector<Bomba*> &bombas);
    bool intentarLanzar(Mapa &mapa, const std::vector<Bomba*> &bombas,
                        int filaDexter, int columnaDexter, int distancia);
    void moverAVecino(Mapa &mapa, const std::vector<Bomba*> &bombas,
                      int filaDexter, int columnaDexter, int modo);
    bool avanzarHaciaDestino(float intervaloTiempo);

public:
    // Formas de moverse al elegir vecino
    static const int HUIR_BOMBA = 0;
    static const int ACERCARSE = 1;
    static const int ALEJARSE = 2;
    static const int PASEAR = 3;

    Mono(int filaInicial, int columnaInicial);

    void actualizar(float intervaloTiempo, Mapa &mapa, const std::vector<Bomba*> &bombas,
                    int filaDexter, int columnaDexter);

    // Comunicacion con el nivel
    bool quiereLanzar() const;
    int getFilaObjetivo() const;
    int getColumnaObjetivo() const;
    void confirmarLanzamiento();

    void setEstrategia(int nuevaEstrategia);
    int getEstrategia() const;

    int getFila() const;
    int getColumna() const;

    void morir();
    bool estaVivo() const;
};

#endif // MONO_H