#ifndef NIVEL2_H
#define NIVEL2_H

#include "mapa.h"
#include "dexter.h"
#include "bomba.h"
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
    void eliminarBomba(int indice);
    void eliminarTodasLasBombas();

    //revisa si alguna bomba tiene fuego en esa casilla
    bool hayFuegoEnCasilla(int fila, int columna) const;

    //casilla donde esta el centro de Dexter
    int getFilaDexter();
    int getColumnaDexter();

public:
    Nivel2(bool dificultad = false);
    ~Nivel2();

    // Deja el nivel como al inicio
    void reiniciar();

    // Avanza el juego un intervalo de tiempo
    void actualizar(int dirX, int dirY, float intervaloTiempo);

    //Dexter pone una bomba en la casilla donde esta parado
    void ponerBomba();

    Mapa &getMapa();
    Dexter &getDexter();
    const std::vector<Bomba*> &getBombas() const;

    bool haPerdido() const;
};

#endif // NIVEL2_H