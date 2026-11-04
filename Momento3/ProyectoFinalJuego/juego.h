#ifndef JUEGO_H
#define JUEGO_H
#include <string>
using namespace std;

class Juego
{
private:
    int nivel;
    int nivelActual;
    int estadoPartida;
    float tiemporestante;
    int dificultad;

public:
    Juego();
    void iniciarJuego();
    void pausa();
    void reanudarJuego();
    void salirJuego();
    void salirNivel();
    void cambioDificultad();
    void seleccionNivel();
};

#endif // JUEGO_H
