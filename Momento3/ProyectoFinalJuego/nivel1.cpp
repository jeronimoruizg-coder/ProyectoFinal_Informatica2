#include "nivel1.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

Nivel1::Nivel1(): inteligenciaGuacamaya(3)
{
    dexter.iniciarNivel1();

    posicionAnteriorDexter = dexter.getX();
    velocidadHorizontalDexter = 0;

    muestrasRecolectadas = 0;
    tiempoGeneracion = 0;

    // iniciamos en dificultad normal
    dificil = true;

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
    tiempoGeneracionGuacamaya = 0;
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

    // Liberamos Guacamayas restantes
    for (int i = 0; i < guacamayas.size(); i++)
    {
        delete guacamayas[i];
    }

    guacamayas.clear();


    for (int i = 0; i < excrementos.size(); i++)
    {
        delete excrementos[i];
    }

    excrementos.clear();
}

// Actualizamos la logica del nivel
void Nivel1::actualizar(int direccion, float intervaloTiempo)
{
    dexter.desplazarse(direccion, intervaloTiempo);
    // Calculamos la velocidad horizontal real de Dexter
    velocidadHorizontalDexter =
        (dexter.getX() - posicionAnteriorDexter)
        / intervaloTiempo;

    // Guardamos la posicion para la siguiente actualizacion
    posicionAnteriorDexter = dexter.getX();
    dexter.aplicarGravedad(intervaloTiempo);
    dexter.actualizarRecoleccion(intervaloTiempo);
    dexter.actualizarInvulnerabilidad(intervaloTiempo);
    dexter.actualizarSuero(intervaloTiempo);
    dexter.actualizarRalentizacion(intervaloTiempo);

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
    actualizarGuacamayas(intervaloTiempo);
    actualizarExcrementos(intervaloTiempo);
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


void Nivel1::generarGuacamaya()
{
    int sentido = 1;

    if (rand() % 2 == 0)
    {
        sentido = -1;
    }

    Guacamaya *guacamaya = new Guacamaya(sentido);

    // Seleccionamos una estrategia en dificultad dificil
    if (dificil)
    {
        guacamaya->asignarEstrategia(
            inteligenciaGuacamaya.elegirOpcion());
    }
    guacamayas.push_back(guacamaya);
}

void Nivel1::eliminarGuacamaya(int indice)
{
    delete guacamayas[indice];

    guacamayas.erase(guacamayas.begin() + indice);
}

void Nivel1::actualizarGuacamayas(float intervaloTiempo)
{
    tiempoGeneracionGuacamaya += intervaloTiempo;

    int frecuencia = 12;

    if (dificil)
    {
        frecuencia = 8;
    }

    // Generamos una nueva guacamaya
    if (tiempoGeneracionGuacamaya >= frecuencia)
    {
        generarGuacamaya();

        tiempoGeneracionGuacamaya -= frecuencia;
    }

    int i = 0;

    while (i < guacamayas.size())
    {
        guacamayas[i]->moverse(intervaloTiempo);

        // Liberamos la memoria cuando sale
        if (guacamayas[i]->salioDelEscenario())
        {
            eliminarGuacamaya(i);
        }
        else
        {
            // Comprobamos si todavia puede lanzar
            if (!guacamayas[i]->haLanzadoExcremento())
            {
                // Solo lanzamos cuando esta dentro del escenario
                if (guacamayas[i]->getX() >= 0 &&
                    guacamayas[i]->getX() +
                            guacamayas[i]->getAncho() <= 1280)
                {
                    // Probabilidad de lanzamiento
                    if (dificil)
                    {
                        // Lanzamiento mediante prediccion
                        if (debeLanzarExcremento(guacamayas[i]))
                        {
                            generarExcremento(guacamayas[i]);
                        }
                    }
                    else
                    {
                        // Lanzamiento aleatorio en dificultad normal
                        if (rand() % 180 == 0)
                        {
                            generarExcremento(guacamayas[i]);
                        }
                    }
                }
            }

            i++;
        }
    }
}

const vector<Guacamaya*>& Nivel1::getGuacamayas() const
{
    return guacamayas;
}


void Nivel1::generarExcremento(Guacamaya *guacamaya)
{
    // Centramos el excremento debajo de la guacamaya
    float posicionX = guacamaya->getX()
                      + guacamaya->getAncho() / 2.0
                      - 12.5;

    float posicionY = guacamaya->getY()
                      + guacamaya->getAlto();

    Excremento *excremento = new Excremento(posicionX, posicionY, guacamaya->getEstrategia());

    excrementos.push_back(excremento);

    // Evitamos que la misma guacamaya vuelva a lanzar
    guacamaya->registrarLanzamiento();
}

void Nivel1::eliminarExcremento(int indice)
{
    delete excrementos[indice];

    excrementos.erase(excrementos.begin() + indice);
}


bool Nivel1::detectarContactoExcremento(
    Excremento *excremento)
{
    return
        dexter.getX() <
            excremento->getX() + excremento->getAncho()

        && dexter.getX() + dexter.getAncho() >
               excremento->getX()

        && dexter.getY() <
               excremento->getY() + excremento->getAlto()

        && dexter.getY() + dexter.getAlto() >
               excremento->getY();
}


void Nivel1::actualizarExcrementos(float intervaloTiempo)
{
    int i = 0;

    while (i < excrementos.size())
    {
        excrementos[i]->actualizar(intervaloTiempo);

        int estrategia = excrementos[i]->getEstrategia();

        // El excremento desaparece sin tocar a Dexter
        if (excrementos[i]->debeDesaparecer())
        {
            if (estrategia != -1)
            {
                inteligenciaGuacamaya.registrarResultado(
                    estrategia, false);
            }

            eliminarExcremento(i);
        }

        // El excremento toca a Dexter
        else if (detectarContactoExcremento(excrementos[i]))
        {
            if (estrategia != -1)
            {
                inteligenciaGuacamaya.registrarResultado(
                    estrategia, true);
            }

            dexter.ralentizar();

            eliminarExcremento(i);
        }
        else
        {
            i++;
        }
    }
}

const vector<Excremento*>& Nivel1::getExcrementos() const
{
    return excrementos;
}


bool Nivel1::debeLanzarExcremento(Guacamaya *guacamaya)
{
    // Centro horizontal de la guacamaya
    float centroGuacamaya =
        guacamaya->getX() + guacamaya->getAncho() / 2.0;

    // Centro horizontal de Dexter
    float objetivo =
        dexter.getX() + dexter.getAncho() / 2.0;

    // Estrategia de anticipacion de medio segundo
    if (guacamaya->getEstrategia() == 1)
    {
        objetivo += velocidadHorizontalDexter * 0.5;
    }

    // Estrategia de anticipacion de un segundo
    else if (guacamaya->getEstrategia() == 2)
    {
        objetivo += velocidadHorizontalDexter;
    }

    // Mantenemos el objetivo dentro del alcance de vuelo
    if (objetivo < 50)
    {
        objetivo = 50;
    }

    if (objetivo > 1230)
    {
        objetivo = 1230;
    }

    // Lanzamos si estamos suficientemente cerca
    return std::abs(centroGuacamaya - objetivo) <= 25;
}