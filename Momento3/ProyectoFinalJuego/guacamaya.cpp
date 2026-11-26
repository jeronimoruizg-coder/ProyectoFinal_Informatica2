#include "guacamaya.h"

Guacamaya::Guacamaya(int sentido)
{
    // Dimensiones fisicas
    ancho = 100;
    alto = 70;

    // Velocidad horizontal de vuelo
    velocidad = 180;

    direccion = sentido;

    // Altura constante de vuelo
    y = 100;

    excrementoLanzado = false;

    // Determinamos el extremo de aparicion
    if (direccion == 1)
    {
        x = -ancho;
    }
    else
    {
        x = 1280;
    }

    //Estrategia de inteligencia
    estrategia = -1;
}

// Comprobamos si abandono el escenario
bool Guacamaya::salioDelEscenario() const
{
    if (direccion == 1)
    {
        return x >= 1280;
    }

    return x + ancho <= 0;
}

// Consultamos si ya realizo un lanzamiento
bool Guacamaya::haLanzadoExcremento() const
{
    return excrementoLanzado;
}

// Registramos que ya realizo el lanzamiento
void Guacamaya::registrarLanzamiento()
{
    excrementoLanzado = true;
}

void Guacamaya::asignarEstrategia(int opcion)
{
    estrategia = opcion;
}

int Guacamaya::getEstrategia() const
{
    return estrategia;
}