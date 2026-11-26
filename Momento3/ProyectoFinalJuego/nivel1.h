#ifndef NIVEL1_H
#define NIVEL1_H

#include "dexter.h"
#include "recolectables.h"
#include <vector>

using namespace std;

class Nivel1
{
private:
    Dexter dexter;

    // Almacenamos las muestras activas
    vector<Recolectables*> muestras;

    int muestrasRecolectadas;
    float tiempoGeneracion;
    bool dificil;

    // Funciones internas para administrar las muestras
    void generarMuestra();
    void actualizarMuestras(float intervaloTiempo);
    void eliminarMuestra(int indice);

    // Comprueba la colision entre la red y una muestra
    bool detectarRecoleccion(Recolectables *muestra);

public:
    Nivel1();
    ~Nivel1();

    void recolectar();
    void actualizar(int direccion, float intervaloTiempo);
    void saltar();

    Dexter &getDexter();

    // Consultamos las muestras sin copiar el vector
    const vector<Recolectables*> &getMuestras();

    int getMuestrasRecolectadas();
};

#endif // NIVEL1_H