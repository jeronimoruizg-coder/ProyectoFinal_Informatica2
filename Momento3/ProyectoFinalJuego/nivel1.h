#ifndef NIVEL1_H
#define NIVEL1_H

#include "dexter.h"
#include "recolectables.h"
#include <vector>
#include "semillaexplosiva.h"
#include "beneficios.h"
#include "hormiga.h"
#include "jabali.h"
#include "guacamaya.h"
#include "excremento.h"
#include "inteligencia.h"

using namespace std;

class Nivel1
{
private:
    Dexter dexter;

    // Tiempo restante de la partida
    float tiempoRestante;

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

    // Objetos beneficiosos
    vector<Beneficios*> beneficios;
    // Tiempos de generacion
    float tiempoGeneracionMedicinal;
    float tiempoGeneracionSuero;
    // Administracion de los beneficios
    void generarBeneficio(int tipo);
    void actualizarBeneficios(float intervaloTiempo);
    void eliminarBeneficio(int indice);

    // Hormigas presentes en el escenario
    vector<Hormiga*> hormigas;
    // Tiempo acumulado para generar hormigas
    float tiempoGeneracionHormigas;
    // Administracion de las hormigas
    void generarHormiga();
    void actualizarHormigas(float intervaloTiempo);
    void eliminarHormiga(int indice);

    // Jabalis presentes en el escenario
    vector<Jabali*> jabalis;
    // Tiempo acumulado para la aparicion
    float tiempoGeneracionJabali;
    // Tiempo restante de la advertencia
    float tiempoAdvertenciaJabali;
    // Direccion del proximo jabali
    int direccionJabali;
    // Administracion de los jabalis
    void generarJabali();
    void actualizarJabalis(float intervaloTiempo);
    void eliminarJabali(int indice);

    // Guacamayas presentes en el escenario
    vector<Guacamaya*> guacamayas;
    // Tiempo acumulado para generar guacamayas
    float tiempoGeneracionGuacamaya;
    // Administracion de las guacamayas
    void generarGuacamaya();
    void actualizarGuacamayas(float intervaloTiempo);
    void eliminarGuacamaya(int indice);

    // Excrementos presentes en el escenario
    vector<Excremento*> excrementos;
    // Administracion de los excrementos
    void generarExcremento(Guacamaya *guacamaya);
    void actualizarExcrementos(float intervaloTiempo);
    void eliminarExcremento(int indice);
    // Deteccion de contacto con Dexter
    bool detectarContactoExcremento(Excremento *excremento);

    // Colision entre Dexter y un enemigo
    bool detectarColisionEnemigo(Enemigos *enemigo);

    // Aprendizaje compartido entre las guacamayas
    Inteligencia inteligenciaGuacamaya;
    // Informacion del movimiento de Dexter
    float posicionAnteriorDexter;
    float velocidadHorizontalDexter;
    // Prediccion de lanzamientos
    bool debeLanzarExcremento(Guacamaya *guacamaya);

public:
    Nivel1(bool dificultad = false);
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

    //Consulta Objetos beneficio
    const vector<Beneficios*>& getBeneficios() const;

    //Consulta Hormigas
    const vector<Hormiga*>& getHormigas() const;

    const vector<Jabali*>& getJabalis() const;
    // Consultas para representar la advertencia
    bool hayAdvertenciaJabali() const;
    int getDireccionJabali() const;

    //Consulta Guacamaya
    const vector<Guacamaya*>& getGuacamayas() const;

    //Consulta Excrementos
    const vector<Excremento*>& getExcrementos() const;

    //Tiempo de partida y condicion de victoria o derrota
    int getTiempoRestante() const;
    bool haGanado() const;
    bool haPerdido() const;
};

#endif // NIVEL1_H