#ifndef NIVEL1_H
#define NIVEL1_H

#include "dexter.h"
#include "recolectables.h"
#include <vector>
#include "semillaexplosiva.h"

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
    bool detectarRecoleccion(Objetos *muestra);

    // Semillas explosivas presentes en el nivel
    vector<SemillaExplosiva*> semillas;
    // Tiempo acumulado para generar semillas
    float tiempoGeneracionSemillas;
    // Funciones para administrar las semillas
    void generarSemilla();
    void actualizarSemillas(float intervaloTiempo);
    void eliminarSemilla(int indice);

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

    // Consultar las semillas desde la vista
    const vector<SemillaExplosiva*>& getSemillas() const;
};

#endif // NIVEL1_H