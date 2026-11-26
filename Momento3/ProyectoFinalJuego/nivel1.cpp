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

    //Tiempo de generacion de beneficios
    tiempoGeneracionMedicinal = 0;
    tiempoGeneracionSuero = 0;

    //Tiempo de generacion de enemigos
    tiempoGeneracionHormigas = 0;
    tiempoGeneracionJabali = 0;
    tiempoAdvertenciaJabali = 0;
    direccionJabali = 1;
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

    // Liberamos los beneficios restantes
    for (int i = 0; i < beneficios.size(); i++)
    {
        delete beneficios[i];
    }

    beneficios.clear();

    // Liberamos las hormigas restantes
    for (int i = 0; i < hormigas.size(); i++)
    {
        delete hormigas[i];
    }

    hormigas.clear();

    // Liberamos los jabalis restantes
    for (int i = 0; i < jabalis.size(); i++)
    {
        delete jabalis[i];
    }

    jabalis.clear();
}

// Actualizamos la logica del nivel
void Nivel1::actualizar(int direccion, float intervaloTiempo)
{
    dexter.desplazarse(direccion, intervaloTiempo);
    dexter.aplicarGravedad(intervaloTiempo);
    dexter.actualizarRecoleccion(intervaloTiempo);
    dexter.actualizarInvulnerabilidad(intervaloTiempo);
    dexter.actualizarSuero(intervaloTiempo);

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
    actualizarBeneficios(intervaloTiempo);
    actualizarHormigas(intervaloTiempo);
    actualizarJabalis(intervaloTiempo);
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

// Generamos un beneficio en una posicion aleatoria
void Nivel1::generarBeneficio(int tipo)
{
    int posicionX = rand() % 1241;

    Beneficios *beneficio =
        new Beneficios(posicionX, tipo);

    beneficios.push_back(beneficio);
}

// Eliminamos el beneficio y liberamos su memoria
void Nivel1::eliminarBeneficio(int indice)
{
    delete beneficios[indice];

    beneficios.erase(beneficios.begin() + indice);
}

void Nivel1::actualizarBeneficios(float intervaloTiempo)
{
    // Acumulamos los tiempos de generacion
    tiempoGeneracionMedicinal += intervaloTiempo;
    tiempoGeneracionSuero += intervaloTiempo;

    // Generamos una planta medicinal cada 15 segundos
    if (tiempoGeneracionMedicinal >= 15)
    {
        generarBeneficio(0);

        tiempoGeneracionMedicinal -= 15;
    }

    // Generamos un suero cada 10 segundos
    if (tiempoGeneracionSuero >= 10)
    {
        generarBeneficio(1);

        tiempoGeneracionSuero -= 10;
    }

    int i = 0;

    while (i < beneficios.size())
    {
        beneficios[i]->actualizar(intervaloTiempo);

        // Si toca el suelo, desaparece
        if (beneficios[i]->llegoAlSuelo())
        {
            eliminarBeneficio(i);
        }

        // Si Dexter lo atrapa, aplicamos el beneficio
        else if (detectarRecoleccion(beneficios[i]))
        {
            if (beneficios[i]->getTipo() == 0)
            {
                dexter.recuperarVida();
            }
            else
            {
                dexter.activarSuero();
            }

            eliminarBeneficio(i);
        }

        else
        {
            i++;
        }
    }
}

const vector<Beneficios*>& Nivel1::getBeneficios() const
{
    return beneficios;
}

void Nivel1::generarHormiga()
{
    // Elegimos aleatoriamente el sentido
    int sentido = 1;

    if (rand() % 2 == 0)
    {
        sentido = -1;
    }

    Hormiga *hormiga = new Hormiga(sentido);

    hormigas.push_back(hormiga);
}

void Nivel1::eliminarHormiga(int indice)
{
    delete hormigas[indice];

    hormigas.erase(hormigas.begin() + indice);
}

bool Nivel1::detectarColisionEnemigo(Enemigos *enemigo)
{
    return
        dexter.getX() <
            enemigo->getX() + enemigo->getAncho()

        && dexter.getX() + dexter.getAncho() >
               enemigo->getX()

        && dexter.getY() <
               enemigo->getY() + enemigo->getAlto()

        && dexter.getY() + dexter.getAlto() >
               enemigo->getY();
}

void Nivel1::actualizarHormigas(float intervaloTiempo)
{
    tiempoGeneracionHormigas += intervaloTiempo;

    int frecuencia = 6;

    // Mayor frecuencia en dificultad dificil
    if (dificil)
    {
        frecuencia = 4;
    }

    if (tiempoGeneracionHormigas >= frecuencia)
    {
        generarHormiga();

        tiempoGeneracionHormigas -= frecuencia;
    }

    int i = 0;

    while (i < hormigas.size())
    {
        hormigas[i]->moverse(intervaloTiempo);

        // Eliminamos las hormigas que salen
        if (hormigas[i]->salioDelEscenario())
        {
            eliminarHormiga(i);
        }
        else
        {
            // Comprobamos el contacto con Dexter
            if (detectarColisionEnemigo(hormigas[i]))
            {
                dexter.recibirDano();
            }

            i++;
        }
    }
}

const vector<Hormiga*>& Nivel1::getHormigas() const
{
    return hormigas;
}


void Nivel1::generarJabali()
{
    Jabali *jabali = new Jabali(direccionJabali);

    jabalis.push_back(jabali);
}

void Nivel1::eliminarJabali(int indice)
{
    delete jabalis[indice];

    jabalis.erase(jabalis.begin() + indice);
}

void Nivel1::actualizarJabalis(float intervaloTiempo)
{
    // Acumulamos el tiempo de generacion
    tiempoGeneracionJabali += intervaloTiempo;

    int frecuencia = 20;

    if (dificil)
    {
        frecuencia = 15;
    }

    // Si existe una advertencia activa
    if (tiempoAdvertenciaJabali > 0)
    {
        tiempoAdvertenciaJabali -= intervaloTiempo;

        // Cuando termina la advertencia aparece el jabali
        if (tiempoAdvertenciaJabali <= 0)
        {
            tiempoAdvertenciaJabali = 0;

            generarJabali();
        }
    }

    // Iniciamos la advertencia dos segundos antes
    // de la aparicion del jabali
    else if (tiempoGeneracionJabali >= frecuencia - 2)
    {
        tiempoGeneracionJabali -= frecuencia;

        // Elegimos aleatoriamente la direccion
        direccionJabali = 1;

        if (rand() % 2 == 0)
        {
            direccionJabali = -1;
        }

        tiempoAdvertenciaJabali = 2;
    }

    // Actualizamos los jabalis existentes
    int i = 0;

    while (i < jabalis.size())
    {
        jabalis[i]->moverse(intervaloTiempo);

        if (jabalis[i]->salioDelEscenario())
        {
            eliminarJabali(i);
        }
        else
        {
            // Reutilizamos la deteccion de colisiones
            if (detectarColisionEnemigo(jabalis[i]))
            {
                dexter.recibirDano();
            }

            i++;
        }
    }
}

const vector<Jabali*>& Nivel1::getJabalis() const
{
    return jabalis;
}

bool Nivel1::hayAdvertenciaJabali() const
{
    return tiempoAdvertenciaJabali > 0;
}

int Nivel1::getDireccionJabali() const
{
    return direccionJabali;
}