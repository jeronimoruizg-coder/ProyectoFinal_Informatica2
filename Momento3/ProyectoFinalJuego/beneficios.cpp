#include "beneficios.h"

// Constructor del objeto beneficioso
Beneficios::Beneficios(float posicionX, int tipoBeneficio)
{
    ancho = 40;
    alto = 40;

    x = posicionX;
    y = -alto;

    velocidadY = 0;

    tipo = tipoBeneficio;
}

// Funcion heredada de Objetos
void Beneficios::actualizar()
{
    actualizar(0.016);
}

// Actualizamos la fisica de caida
void Beneficios::actualizar(float intervaloTiempo)
{
    velocidadY += 180 * intervaloTiempo;

    y += velocidadY * intervaloTiempo;
}

// Comprobamos si alcanzo el suelo
bool Beneficios::llegoAlSuelo() const
{
    return y + alto >= 650;
}

// Consultamos el tipo de beneficio
int Beneficios::getTipo() const
{
    return tipo;
}

//------------------------------------------------ nivel 2 ------------------------------------//

// Beneficio del nivel 2: queda quieto en su casilla, escondido
Beneficios::Beneficios(int filaInicial, int columnaInicial, int tipoBeneficio)
{
    fila = filaInicial;
    columna = columnaInicial;
    tipo = tipoBeneficio;

    // Arranca escondido debajo de la pared
    oculto = true;
    velocidadY = 0;

    // Un poco mas pequeno que la casilla y centrado
    ancho = 28;
    alto = 28;
    x = columna * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    y = fila * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;
}

void Beneficios::actualizar(Mapa &mapa)
{
    // Si la pared que lo tapaba ya es pasillo, se deja ver
    if (oculto && mapa.getCasilla(fila, columna) == Mapa::PASILLO)
    {
        oculto = false;
    }
}

bool Beneficios::estaOculto() const { return oculto; }
int Beneficios::getFila() const { return fila; }
int Beneficios::getColumna() const { return columna; }