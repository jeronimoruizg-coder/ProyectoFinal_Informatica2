#include "dexter.h"

Dexter::Dexter()
{
    ancho = 36;
    alto = 36;
    velocidad = 3;
    velocidadY = 0;

    // arranca centrado en la casilla (1, 1)
    x = 1 * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    y = 1 * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;

    //Nivel 1
    // Inicializamos el estado de recoleccion
    tiempoRecoleccion = 0;
    orientacion = 1;

    vidas = 3;
    tiempoInvulnerabilidad = 0;
    tiempoSuero = 0;
}

void Dexter::mover(int dirX, int dirY, Mapa &mapa)
{
    // cada eje se revisa por separado para que pueda deslizarse por las paredes
    float nuevaX = x + dirX * velocidad;
    if (!chocaConPared(nuevaX, y, mapa)) {
        x = nuevaX;
    }

    float nuevaY = y + dirY * velocidad;
    if (!chocaConPared(x, nuevaY, mapa)) {
        y = nuevaY;
    }
}

bool Dexter::chocaConPared(float nuevaX, float nuevaY, Mapa &mapa)
{
    int tam = Mapa::TAM_CASILLA;

    // casillas donde caerian las 4 esquinas de Dexter
    int columnaIzq = nuevaX / tam;
    int columnaDer = (nuevaX + ancho - 1) / tam;
    int filaArriba = nuevaY / tam;
    int filaAbajo = (nuevaY + alto - 1) / tam;

    return mapa.esPared(filaArriba, columnaIzq) || mapa.esPared(filaArriba, columnaDer)
           || mapa.esPared(filaAbajo, columnaIzq) || mapa.esPared(filaAbajo, columnaDer);
}

float Dexter::getX() { return x; }
float Dexter::getY() { return y; }
int Dexter::getAncho() { return ancho; }
int Dexter::getAlto() { return alto; }




//---------------------------------//-----------------------------------//

//Nivel 1
void Dexter::iniciarNivel1() //Modificamos los atributos de dexter para el nivel 1
{
    ancho = 50;
    alto = 80;
    x = 200;
    y = 650 - alto;
    velocidad = 300;
    velocidadY = 0;
    // Inicializamos el estado de recoleccion
    tiempoRecoleccion = 0;
    orientacion = 1;
    //Inicializamos las vidas
    vidas = 3;
    tiempoInvulnerabilidad = 0;
    tiempoSuero = 0;
}

void Dexter::desplazarse(int direccion, float intervaloTiempo)
{
    // Actualizamos la orientacion cuando Dexter se mueve
    if (direccion > 0)
    {
        orientacion = 1;
    }
    else if (direccion < 0)
    {
        orientacion = -1;
    }
    // Calculamos la nueva posicion horizontal
    x = x + direccion * velocidad * intervaloTiempo; //Primer Calculo Fisico (MRU)

    // Limite izquierdo del escenario
    if (x < 0)
    {
        x = 0; //Mantenemos a Dexter dentro de los limites
    }

    // Limite derecho del escenario
    if (x + ancho > 1280)
    {
        x = 1280 - ancho; //Mantenemos a Dexter dentro de los limites
    }
}

void Dexter::saltar()
{
    // Solo puede saltar si esta sobre el suelo
    if (y + alto >= 650)
    {
        velocidadY = -550;
    }
}

void Dexter::aplicarGravedad(float intervaloTiempo)
{
    // Aceleracion de la gravedad
    velocidadY = velocidadY + 1500 * intervaloTiempo; //Segunda Fisica (MRUA)

    // Actualizamos la posicion vertical
    y = y + velocidadY * intervaloTiempo;

    // Detectamos el contacto con el suelo
    if (y + alto >= 650)
    {
        y = 650 - alto;
        velocidadY = 0;
    }
}

// mecanica de recoleccion
void Dexter::atraparObjetos()
{
    // Iniciamos una recoleccion solamente si
    // no hay otra en ejecucion
    if (tiempoRecoleccion <= 0)
    {
        tiempoRecoleccion = 0.4;
    }
}

// Actualizamos la duracion de la recoleccion
void Dexter::actualizarRecoleccion(float intervaloTiempo)
{
    if (tiempoRecoleccion > 0)
    {
        tiempoRecoleccion -= intervaloTiempo;

        // Evitamos que el tiempo sea negativo
        if (tiempoRecoleccion < 0)
        {
            tiempoRecoleccion = 0;
        }
    }
}

// Comprobamos si Dexter esta utilizando la red
bool Dexter::estaRecolectando()
{
    return tiempoRecoleccion > 0;
}

// Calculamos la posicion horizontal de la zona de captura
float Dexter::getRecoleccionX()
{
    // Si Dexter mira a la derecha,
    // la zona aparece un poco delante de el,
    // pero montandose ligeramente sobre su cuerpo
    if (orientacion == 1)
    {
        return x + ancho - 15;
    }

    // Si mira a la izquierda,
    // hacemos lo mismo pero hacia el otro lado
    return x - getRecoleccionAncho() + 15;
}

// Calculamos la posicion vertical de la zona de captura
float Dexter::getRecoleccionY()
{

    return y - 20;
}

// Ancho de la zona de captura
int Dexter::getRecoleccionAncho()
{

    return 55;
}

// Alto de la zona de captura
int Dexter::getRecoleccionAlto()
{
    // Tambien reducimos la altura
    // para que no quede tan exagerada
    return 60;
}


// Aplicamos daño a Dexter
void Dexter::recibirDano()
{
    // No recibe daño si es invulnerable
    // o si ya no tiene vidas
    if (esInvulnerable() || vidas <= 0)
    {
        return;
    }

    // Perdemos una vida
    vidas--;

    // Activamos dos segundos de invulnerabilidad
    tiempoInvulnerabilidad = 2;
}

// Actualizamos el tiempo de invulnerabilidad
void Dexter::actualizarInvulnerabilidad(float intervaloTiempo)
{
    if (tiempoInvulnerabilidad > 0)
    {
        tiempoInvulnerabilidad -= intervaloTiempo;

        // Evitamos valores negativos
        if (tiempoInvulnerabilidad < 0)
        {
            tiempoInvulnerabilidad = 0;
        }
    }
}

// Consultamos las vidas restantes
int Dexter::getVidas() const
{
    return vidas;
}

// Comprobamos si Dexter es invulnerable
bool Dexter::esInvulnerable() const
{
    return tiempoInvulnerabilidad > 0;
}

// Determinamos cuando Dexter debe verse rojo
bool Dexter::debeTitilarRojo() const
{
    if (!esInvulnerable())
    {
        return false;
    }

    int intervalo = int(
        (2 - tiempoInvulnerabilidad) / 0.15);

    return intervalo % 2 == 0;
}

// Recuperamos una vida sin superar el maximo
void Dexter::recuperarVida()
{
    if (vidas > 0 && vidas < 3)
    {
        vidas++;
    }
}

// Activamos el aumento temporal de velocidad
void Dexter::activarSuero()
{
    velocidad = 450;

    tiempoSuero = 8;
}

// Actualizamos la duracion del suero
void Dexter::actualizarSuero(float intervaloTiempo)
{
    if (tiempoSuero > 0)
    {
        tiempoSuero -= intervaloTiempo;

        // Si termina el efecto, restauramos la velocidad
        if (tiempoSuero <= 0)
        {
            tiempoSuero = 0;

            velocidad = 300;
        }
    }
}

// Consultamos si el efecto sigue activo
bool Dexter::tieneSuero() const
{
    return tiempoSuero > 0;
}