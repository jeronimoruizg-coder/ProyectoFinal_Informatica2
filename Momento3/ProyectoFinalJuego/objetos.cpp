#include "objetos.h"

// Constructor de la clase padre
Objetos::Objetos()
{
    // Inicializamos los atributos comunes
    x = 0;
    y = 0;
    ancho = 0;
    alto = 0;
}

// Devuelve la posicion horizontal
float Objetos::getX()
{
    return x;
}

// Devuelve la posicion vertical
float Objetos::getY()
{
    return y;
}

// Devuelve el ancho fisico del objeto
int Objetos::getAncho()
{
    return ancho;
}

// Devuelve el alto fisico del objeto
int Objetos::getAlto()
{
    return alto;
}