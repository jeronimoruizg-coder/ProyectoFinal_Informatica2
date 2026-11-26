#include "nivel1.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Nivel1::Nivel1()
{
    dexter.iniciarNivel1();

    muestrasRecolectadas = 0;
    tiempoGeneracion = 0;

    // iniciamos en dificultad normal
    dificil = false;

    // Inicializamos la generacion de numeros aleatorios
    srand(time(0));

    //Semillas
    tiempoGeneracionSemillas = 0;
}


Nivel1::~Nivel1()
{
    // Liberamos las muestras que queden al cerrar el nivel
    for (int i = 0; i < muestras.size(); i++)
    {
        delete muestras[i];
    }

    muestras.clear();

    // Liberamos las semillas restantes
    for (int i = 0; i < semillas.size(); i++)
    {
        delete semillas[i];
    }

    semillas.clear();
}

// Actualizamos la logica del nivel
void Nivel1::actualizar(int direccion, float intervaloTiempo)
{
    dexter.desplazarse(direccion, intervaloTiempo);
    dexter.aplicarGravedad(intervaloTiempo);
    dexter.actualizarRecoleccion(intervaloTiempo);
    dexter.actualizarInvulnerabilidad(intervaloTiempo);

    // Acumulamos el tiempo para generar nuevas muestras
    tiempoGeneracion += intervaloTiempo;

    // Generamos una muestra cada 1.2 segundos
    if (tiempoGeneracion >= 1.2)
    {
        generarMuestra();
        tiempoGeneracion -= 1.2;
    }

    actualizarMuestras(intervaloTiempo);
    actualizarSemillas(intervaloTiempo);
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
bool Nivel1::detectarRecoleccion(Objetos *muestra)
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

//------------------------------------ Semillas --------------------------------//
// Generamos una semilla en una posicion aleatoria
void Nivel1::generarSemilla()
{
    int posicionX = rand() % 1241;

    SemillaExplosiva *semilla =
        new SemillaExplosiva(posicionX);

    semillas.push_back(semilla);
}

// Eliminamos una semilla y liberamos su memoria
void Nivel1::eliminarSemilla(int indice)
{
    delete semillas[indice];

    semillas.erase(semillas.begin() + indice);
}

// Actualizamos todas las semillas del nivel
void Nivel1::actualizarSemillas(float intervaloTiempo)
{
    // Acumulamos el tiempo para generar semillas
    tiempoGeneracionSemillas += intervaloTiempo;

    if (tiempoGeneracionSemillas >= 5.0)
    {
        generarSemilla();

        tiempoGeneracionSemillas -= 5.0;
    }

    int i = 0;

    while (i < semillas.size())
    {
        // Actualizamos la caida o la explosion
        semillas[i]->actualizar(intervaloTiempo);

        // Si ya exploto, esperamos que termine
        if (semillas[i]->haExplotado())
        {
            if (semillas[i]->terminoExplosion())
            {
                eliminarSemilla(i);
            }
            else
            {
                i++;
            }
        }

        // Si llega al suelo, desaparece sin explotar
        else if (semillas[i]->llegoAlSuelo())
        {
            eliminarSemilla(i);
        }

        // Si Dexter la atrapa, comienza la explosion
        else if (detectarRecoleccion(semillas[i]))
        {
            semillas[i]->explotar();

            // Aplicamos daño a Dexter
            dexter.recibirDano();

            i++;
        }

        else
        {
            i++;
        }
    }
}

//Getter de semilla
const vector<SemillaExplosiva*>& Nivel1::getSemillas() const
{
    return semillas;
}

