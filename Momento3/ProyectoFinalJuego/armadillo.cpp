#include "armadillo.h"
#include <cstdlib>

Armadillo::Armadillo(int filaInicial, int columnaInicial)
{
    ancho = 40;
    alto = 40;
    velocidad = 90;            // caminando

    // Valores de la embestida
    velocidadEmbestida = 0;
    aceleracion = 900;         // pixeles por segundo al cuadrado
    velocidadMaxima = 480;
    direccion = 1;
    vivo = true;

    fila = filaInicial;
    columna = columnaInicial;
    filaDestino = fila;
    columnaDestino = columna;

    // Arranca centrado en su casilla
    x = columna * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    y = fila * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;

    dirFila = 0;
    dirColumna = 1;

    estado = PATRULLANDO;
    tiempoEstado = 0;
}

void Armadillo::actualizar(float intervaloTiempo, Mapa &mapa, int filaDexter, int columnaDexter)
{
    if (estado == PATRULLANDO)
    {
        // Solo toma decisiones cuando llega al centro de una casilla
        if (avanzarHaciaDestino(intervaloTiempo, velocidad))
        {
            if (puedeVerADexter(mapa, filaDexter, columnaDexter))
            {
                // Se queda quieto tomando impulso
                estado = PREPARANDO;
                tiempoEstado = 0.6;
            }
            else
            {
                elegirDireccionPatrulla(mapa);
            }
        }
    }
    else if (estado == PREPARANDO)
    {
        tiempoEstado -= intervaloTiempo;

        if (tiempoEstado <= 0)
        {
            // Arranca la embestida desde una velocidad baja
            estado = EMBISTIENDO;
            velocidadEmbestida = 60;
            siguienteCasillaEmbestida(mapa);
        }
    }
    else if (estado == EMBISTIENDO)
    {
        // Va ganando velocidad hasta el maximo: v = v0 + a*t
        velocidadEmbestida += aceleracion * intervaloTiempo;
        if (velocidadEmbestida > velocidadMaxima)
        {
            velocidadEmbestida = velocidadMaxima;
        }

        // Al llegar a cada casilla revisa que hay adelante
        if (avanzarHaciaDestino(intervaloTiempo, velocidadEmbestida))
        {
            siguienteCasillaEmbestida(mapa);
        }
    }
    else if (estado == ATURDIDO)
    {
        tiempoEstado -= intervaloTiempo;

        if (tiempoEstado <= 0)
        {
            estado = PATRULLANDO;
            elegirDireccionPatrulla(mapa);
        }
    }
}

// Mueve al armadillo hacia el centro de la casilla destino.
// Devuelve true cuando ya llego.
bool Armadillo::avanzarHaciaDestino(float intervaloTiempo, float rapidez)
{
    float destinoX = columnaDestino * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    float destinoY = filaDestino * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;
    float paso = rapidez * intervaloTiempo;

    // Avanzamos en X sin pasarnos del destino
    if (x < destinoX)
    {
        x += paso;
        if (x > destinoX) x = destinoX;
    }
    else if (x > destinoX)
    {
        x -= paso;
        if (x < destinoX) x = destinoX;
    }

    // Lo mismo en Y
    if (y < destinoY)
    {
        y += paso;
        if (y > destinoY) y = destinoY;
    }
    else if (y > destinoY)
    {
        y -= paso;
        if (y < destinoY) y = destinoY;
    }

    if (x == destinoX && y == destinoY)
    {
        fila = filaDestino;
        columna = columnaDestino;
        return true;
    }

    return false;
}

void Armadillo::elegirDireccionPatrulla(Mapa &mapa)
{
    // Si puede seguir derecho, sigue
    if (!mapa.esPared(fila + dirFila, columna + dirColumna))
    {
        filaDestino = fila + dirFila;
        columnaDestino = columna + dirColumna;
        return;
    }

    // Si no, busca al azar una direccion libre
    int cambioFila[4] = {-1, 1, 0, 0};
    int cambioColumna[4] = {0, 0, -1, 1};
    int libres[4];
    int cantidadLibres = 0;

    for (int d = 0; d < 4; d++)
    {
        if (!mapa.esPared(fila + cambioFila[d], columna + cambioColumna[d]))
        {
            libres[cantidadLibres] = d;
            cantidadLibres++;
        }
    }

    // Encerrado: se queda quieto
    if (cantidadLibres == 0)
    {
        return;
    }

    int elegida = libres[rand() % cantidadLibres];
    dirFila = cambioFila[elegida];
    dirColumna = cambioColumna[elegida];
    filaDestino = fila + dirFila;
    columnaDestino = columna + dirColumna;
}

// Dexter esta en su fila o columna, cerca y sin paredes fijas en medio
bool Armadillo::puedeVerADexter(Mapa &mapa, int filaDexter, int columnaDexter)
{
    int distanciaMaxima = 6;

    if (filaDexter != fila && columnaDexter != columna)
    {
        return false;
    }

    int distancia = abs(filaDexter - fila) + abs(columnaDexter - columna);

    if (distancia == 0 || distancia > distanciaMaxima)
    {
        return false;
    }

    // Direccion hacia Dexter
    int pasoFila = 0;
    int pasoColumna = 0;
    if (filaDexter > fila) pasoFila = 1;
    if (filaDexter < fila) pasoFila = -1;
    if (columnaDexter > columna) pasoColumna = 1;
    if (columnaDexter < columna) pasoColumna = -1;

    // Las paredes rompibles no le importan (las rompe), las fijas si lo tapan
    for (int i = 1; i < distancia; i++)
    {
        int tipo = mapa.getCasilla(fila + pasoFila * i, columna + pasoColumna * i);
        if (tipo == Mapa::PARED_FIJA || tipo == Mapa::PUERTA)
        {
            return false;
        }
    }

    dirFila = pasoFila;
    dirColumna = pasoColumna;
    return true;
}

// Decide a donde sigue la embestida segun lo que haya adelante
void Armadillo::siguienteCasillaEmbestida(Mapa &mapa)
{
    int siguienteFila = fila + dirFila;
    int siguienteColumna = columna + dirColumna;
    int tipo = mapa.getCasilla(siguienteFila, siguienteColumna);

    // Pared rompible: la destruye de un golpe y ahi termina la embestida
    if (tipo == Mapa::PARED_ROMPIBLE)
    {
        mapa.romperPared(siguienteFila, siguienteColumna);
        terminarEmbestida();
        return;
    }

    // Pared fija o puerta: se estrella
    if (tipo == Mapa::PARED_FIJA || tipo == Mapa::PUERTA)
    {
        terminarEmbestida();
        return;
    }

    // Pasillo: sigue derecho
    filaDestino = siguienteFila;
    columnaDestino = siguienteColumna;
}

// Despues del golpe queda aturdido un momento
void Armadillo::terminarEmbestida()
{
    estado = ATURDIDO;
    tiempoEstado = 1.0;
    velocidadEmbestida = 0;

    // Se queda en la casilla donde va (o donde esta)
    filaDestino = fila;
    columnaDestino = columna;
}

void Armadillo::morir()
{
    vivo = false;
}

bool Armadillo::estaVivo() const
{
    return vivo;
}

bool Armadillo::estaPreparando() const
{
    return estado == PREPARANDO;
}

bool Armadillo::estaAturdido() const
{
    return estado == ATURDIDO;
}

bool Armadillo::estaEmbistiendo() const
{
    return estado == EMBISTIENDO;
}