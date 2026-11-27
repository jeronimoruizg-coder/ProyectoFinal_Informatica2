#ifndef ARMADILLO_H
#define ARMADILLO_H

#include "enemigos.h"
#include "mapa.h"
#include "bomba.h"
#include <vector>

// Enemigo del nivel 2 que embiste en linea recta y rompe paredes.
// Se mueve casilla por casilla por los pasillos.
class Armadillo : public Enemigos
{
private:
    // Estados posibles
    static const int PATRULLANDO = 0;
    static const int PREPARANDO = 1;
    static const int EMBISTIENDO = 2;
    static const int ATURDIDO = 3;

    int estado;
    float tiempoEstado;   // tiempo que falta para salir de preparando o aturdido

    // Casilla donde esta y casilla a la que se dirige
    int fila, columna;
    int filaDestino, columnaDestino;

    // Direccion en casillas: (-1,0) arriba, (1,0) abajo, (0,-1) izquierda, (0,1) derecha
    int dirFila, dirColumna;

    // La embestida arranca lenta y acelera (MRUA)
    float velocidadEmbestida;
    float aceleracion;
    float velocidadMaxima;

    bool vivo;

    bool avanzarHaciaDestino(float intervaloTiempo, float rapidez);
    void elegirDireccionPatrulla(Mapa &mapa, const std::vector<Bomba*> &bombas);
    bool puedeVerADexter(Mapa &mapa, int filaDexter, int columnaDexter);
    void siguienteCasillaEmbestida(Mapa &mapa, const std::vector<Bomba*> &bombas);

    // Una casilla esta bloqueada si es pared o tiene una bomba sin explotar
    bool hayBombaEn(const std::vector<Bomba*> &bombas, int filaConsulta, int columnaConsulta);
    bool estaBloqueada(Mapa &mapa, const std::vector<Bomba*> &bombas,
                       int filaConsulta, int columnaConsulta);

public:
    Armadillo(int filaInicial, int columnaInicial);

    void actualizar(float intervaloTiempo, Mapa &mapa, const std::vector<Bomba*> &bombas,
                    int filaDexter, int columnaDexter);

    // Termina la embestida (por ejemplo, al golpear a Dexter)
    void terminarEmbestida();

    void morir();
    bool estaVivo() const;
    bool estaPreparando() const;
    bool estaAturdido() const;
    bool estaEmbistiendo() const;
};

#endif // ARMADILLO_H