#ifndef NIVEL2_H
#define NIVEL2_H

#include "mapa.h"
#include "dexter.h"
#include "bomba.h"
#include "niebla.h"
#include "armadillo.h"
#include "mono.h"
#include "inteligencia.h"
#include "beneficios.h"
#include "disparoveneno.h"
#include <vector>

class Nivel2
{
private:
    Mapa mapa;
    Dexter dexter;
    bool dificil;

    //bombas que hay en el mapa (con mecha o explotando)
    std::vector<Bomba*> bombas;

    void actualizarBombas(float intervaloTiempo);
    void explotarBomba(int indice);
    void eliminarBomba(int indice);
    void eliminarTodasLasBombas();

    //revisa si alguna bomba tiene fuego en esa casilla
    bool hayFuegoEnCasilla(int fila, int columna) const;

    // Mueve a Dexter sin dejarlo atravesar bombas
    void moverDexter(int dirX, int dirY, float intervaloTiempo);
    bool dexterChocaConBomba();

    //casilla donde esta el centro de Dexter
    int getFilaDexter();
    int getColumnaDexter();

    // Enemigos
    std::vector<Niebla*> nieblas;
    std::vector<Armadillo*> armadillos;
    std::vector<Mono*> monos;

    // "Cerebro" de mono
    Inteligencia inteligenciaMono;

    void crearEnemigos();
    void actualizarNieblas(float intervaloTiempo);
    void actualizarArmadillos(float intervaloTiempo);
    void actualizarMonos(float intervaloTiempo);
    void eliminarTodosLosEnemigos();

    // Revisa si un rectangulo se cruza con Dexter
    bool tocaADexter(float x, float y, int ancho, int alto);

    // Abre la puerta cuando ya no quedan enemigos
    void revisarPuerta();

    // Empujon del armadillo y choques entre enemigos
    void empujarDexter(float intervaloTiempo);
    bool seTocan(float x1, float y1, int ancho1, int alto1,
                 float x2, float y2, int ancho2, int alto2);

    // Power-ups escondidos en las paredes rompibles
    std::vector<Beneficios*> beneficios;
    void crearBeneficios();
    void actualizarBeneficios();

    // Disparos de la pistola de veneno
    std::vector<DisparoVeneno*> disparos;
    void actualizarDisparos(float intervaloTiempo);
    bool disparoAlcanza(float x, float y, int ancho, int alto);

    void eliminarBeneficiosYDisparos();

public:
    Nivel2(bool dificultad = false);
    ~Nivel2();

    // Deja el nivel como al inicio
    void reiniciar();

    // Avanza el juego un intervalo de tiempo
    void actualizar(int dirX, int dirY, float intervaloTiempo);

    //Dexter pone una bomba en la casilla donde esta parado
    void ponerBomba();

    // Dexter dispara si tiene la pistola de veneno
    void disparar();

    Mapa &getMapa();
    Dexter &getDexter();
    const std::vector<Bomba*> &getBombas() const;
    const std::vector<Niebla*> &getNieblas() const;
    const std::vector<Armadillo*> &getArmadillos() const;
    const std::vector<Mono*> &getMonos() const;
    const std::vector<Beneficios*> &getBeneficios() const;
    const std::vector<DisparoVeneno*> &getDisparos() const;

    bool haPerdido() const;
    bool haGanado();
};

#endif // NIVEL2_H