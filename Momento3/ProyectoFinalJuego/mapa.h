#ifndef MAPA_H
#define MAPA_H

class Mapa
{
public:
    static const int FILAS = 13;
    static const int COLUMNAS = 15;
    static const int TAM_CASILLA = 48;   // pixeles de cada casilla

    // tipos de casilla
    static const int PASILLO = 0;
    static const int PARED_FIJA = 1;
    static const int PARED_ROMPIBLE = 2;
    static const int PUERTA = 3;

    //constructor
    Mapa();
    void reiniciar();

    int getCasilla(int fila, int columna);
    bool esPared(int fila, int columna);
    void romperPared(int fila, int columna);
    void abrirPuerta();
    bool estaPuertaAbierta();

private:
    static const int DISENO[FILAS][COLUMNAS];
    int casillas[FILAS][COLUMNAS];
    bool puertaAbierta;
};

#endif // MAPA_H