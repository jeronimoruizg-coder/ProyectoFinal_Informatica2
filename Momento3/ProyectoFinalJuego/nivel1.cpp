#include "nivel1.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Nivel1::Nivel1()
{
    dexter.iniciarNivel1();

    muestrasRecolectadas = 0;
    tiempoGeneracion = 0;

    // Por ahora iniciamos en dificultad normal
    dificil = false;

    // Inicializamos la generacion de numeros aleatorios
    srand(time(0));
}

// Liberamos las muestras que queden al cerrar el nivel
Nivel1::~Nivel1()
{
    for (int i = 0; i < muestras.size(); i++)
    {
        delete muestras[i];
    }

    muestras.clear();
}

// Actualizamos la logica del nivel
void Nivel1::actualizar(int direccion,
                        float intervaloTiempo)
{
    dexter.desplazarse(direccion, intervaloTiempo);
    dexter.aplicarGravedad(intervaloTiempo);
    dexter.actualizarRecoleccion(intervaloTiempo);

    // Acumulamos el tiempo para generar nuevas muestras
    tiempoGeneracion += intervaloTiempo;

    // Generamos una muestra cada 1.2 segundos
    if (tiempoGeneracion >= 1.2)
    {
        generarMuestra();
        tiempoGeneracion -= 1.2;
    }

    actualizarMuestras(intervaloTiempo);
}

// Solicitamos a Dexter saltar
void Nivel1::saltar()
{
    dexter.saltar();
}

// Consultamos el personaje
Dexter &Nivel1::getDexter()
{
    return dexter;
}

// Solicitamos a Dexter utilizar la red
void Nivel1::recolectar()
{
    dexter.atraparObjetos();
}

// Creamos una nueva muestra botanica
void Nivel1::generarMuestra()
{
    // Elegimos una posicion horizontal aleatoria
    int posicionX = rand() % 1241;

    // Probabilidad del 20% de generar una muestra especial
    bool especial = rand() % 5 == 0;

    // Creamos la muestra mediante memoria dinamica
    Recolectables *nuevaMuestra =
        new Recolectables(posicionX, especial, dificil);

    // Guardamos su direccion en el vector
    muestras.push_back(nuevaMuestra);
}

// Actualizamos todas las muestras existentes
void Nivel1::actualizarMuestras(float intervaloTiempo)
{
    int i = 0;

    while (i < muestras.size())
    {
        muestras[i]->actualizar(intervaloTiempo);

        // Eliminamos la muestra si toca el suelo
        if (muestras[i]->llegoAlSuelo())
        {
            eliminarMuestra(i);
        }

        // Comprobamos si Dexter logro atraparla
        else if (detectarRecoleccion(muestras[i]))
        {
            muestrasRecolectadas += muestras[i]->sumarPuntos();

            eliminarMuestra(i);
        }

        else
        {
            // Avanzamos solamente si no eliminamos la muestra
            i++;
        }
    }
}

// Eliminamos una muestra y liberamos su memoria
void Nivel1::eliminarMuestra(int indice)
{
    delete muestras[indice];

    muestras.erase(muestras.begin() + indice);
}

// Detectamos la interseccion entre dos rectangulos
bool Nivel1::detectarRecoleccion(Recolectables *muestra)
{
    // Si Dexter no utiliza la red, no puede recolectar
    if (!dexter.estaRecolectando())
    {
        return false;
    }

    // Posicion y dimensiones de la red
    float redX = dexter.getRecoleccionX();
    float redY = dexter.getRecoleccionY();

    int redAncho = dexter.getRecoleccionAncho();
    int redAlto = dexter.getRecoleccionAlto();

    // Comprobamos si los rectangulos se superponen
    return redX < muestra->getX() + muestra->getAncho()
           && redX + redAncho > muestra->getX()
           && redY < muestra->getY() + muestra->getAlto()
           && redY + redAlto > muestra->getY();
}

// Consultamos las muestras activas
const vector<Recolectables*> &Nivel1::getMuestras()
{
    return muestras;
}

// Consultamos la cantidad recolectada
int Nivel1::getMuestrasRecolectadas()
{
    return muestrasRecolectadas;
}