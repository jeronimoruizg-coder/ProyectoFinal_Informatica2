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
    tiempoRalentizacion = 0;
    vidas = 3;
    tiempoInvulnerabilidad = 0;
    tiempoSuero = 0;
}

void Dexter::mover(int dirX, int dirY, float intervaloTiempo, Mapa &mapa)
{
    // distancia que avanza en este intervalo (MRU)
    float paso = velocidad * intervaloTiempo;


    // Con el suero camina al doble de velocidad
    if (tiempoSuero > 0) {
        paso = paso * 2;
    }

    // Guardamos hacia donde mira para saber hacia donde dispara
    if (dirX != 0 || dirY != 0) {
        mirandoX = dirX;
        mirandoY = dirY;
    }

    // cada eje se revisa por separado para que pueda deslizarse por las paredes
    float nuevaX = x + dirX * paso;
    if (!chocaConPared(nuevaX, y, mapa)) {
        x = nuevaX;
    }

    float nuevaY = y + dirY * paso;
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
    //Inicializamos el tiempo de ralentizacion
    tiempoRalentizacion = 0;
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


    // Calculamos la velocidad real de movimiento
    float velocidadActual = velocidad;

    // Reducimos la velocidad si Dexter esta ralentizado
    if (estaRalentizado())
    {
        velocidadActual *= 0.5;
    }

    // Calculamos la nueva posicion horizontal
    x = x + direccion * velocidadActual * intervaloTiempo; //Primer Calculo Fisico (MRU)

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


// Activamos la ralentizacion durante dos segundos
void Dexter::ralentizar()
{
    tiempoRalentizacion = 2;
}

// Actualizamos la duracion del efecto
void Dexter::actualizarRalentizacion(float intervaloTiempo)
{
    if (tiempoRalentizacion > 0)
    {
        tiempoRalentizacion -= intervaloTiempo;

        if (tiempoRalentizacion < 0)
        {
            tiempoRalentizacion = 0;
        }
    }
}

// Consultamos si Dexter esta ralentizado
bool Dexter::estaRalentizado() const
{
    return tiempoRalentizacion > 0;
}

//---------------------------------//-----------------------------------//

//Nivel 2
void Dexter::iniciarNivel2() //Modificamos los atributos de dexter para el laberinto
{

    ancho = 36;
    alto = 36;

    // arranca centrado en la casilla (1, 1)
    x = 1 * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - ancho) / 2;
    y = 1 * Mapa::TAM_CASILLA + (Mapa::TAM_CASILLA - alto) / 2;

    velocidad = 180;   // pixeles por segundo
    velocidadY = 0;
    orientacion = 1;
    tiempoRecoleccion = 0;

    vidas = 3;
    tiempoInvulnerabilidad = 0;
    tiempoSuero = 0;
    tiempoRalentizacion = 0;

    // Sin power-ups al empezar y mirando hacia abajo
    tiempoPistola = 0;
    recargaDisparo = 0;
    disparosRestantes = 0;
    mirandoX = 0;
    mirandoY = 1;

    // Sin empujon al empezar
    impulsoX = 0;
    impulsoY = 0;

}


// Permite devolver a Dexter cuando choca con una bomba
void Dexter::setPosicion(float nuevaX, float nuevaY)
{
    x = nuevaX;
    y = nuevaY;
}

//------------------------------------------------ Power-ups nivel 2 ------------------------------------//

// El suero del nivel 2 dura 15 s. No tocamos la velocidad:
// mover() mira si queda suero y duplica el paso
void Dexter::activarSueroNivel2()
{
    tiempoSuero = 15;
}

// La pistola de veneno dura 25 s o 5 disparos, lo que pase primero
void Dexter::activarPistola()
{
    tiempoPistola = 25;
    recargaDisparo = 0;
    disparosRestantes = 5;
}

// Descuenta el tiempo de los power-ups y de la recarga
void Dexter::actualizarPoderes(float intervaloTiempo)
{
    if (tiempoSuero > 0)
    {
        tiempoSuero -= intervaloTiempo;
        if (tiempoSuero < 0) tiempoSuero = 0;
    }

    if (tiempoPistola > 0)
    {
        tiempoPistola -= intervaloTiempo;
        if (tiempoPistola < 0) tiempoPistola = 0;
    }

    if (recargaDisparo > 0)
    {
        recargaDisparo -= intervaloTiempo;
    }
}

bool Dexter::tienePistola() const
{
    return tiempoPistola > 0;
}

// Solo puede disparar una vez por segundo
bool Dexter::puedeDisparar() const
{
    return tiempoPistola > 0 && recargaDisparo <= 0;
}

void Dexter::registrarDisparo()
{
    recargaDisparo = 1;
    disparosRestantes--;

    // Si ya no le quedan disparos, se queda sin pistola
    if (disparosRestantes <= 0)
    {
        tiempoPistola = 0;
    }
}

int Dexter::getDisparosRestantes() const
{
    return disparosRestantes;
}

float Dexter::getTiempoSuero() const { return tiempoSuero; }
float Dexter::getTiempoPistola() const { return tiempoPistola; }
int Dexter::getMirandoX() const { return mirandoX; }
int Dexter::getMirandoY() const { return mirandoY; }

//------------------------------------------------ Empujon nivel 2 ------------------------------------//

// El golpe le da a Dexter una velocidad de golpe (impulso)
void Dexter::aplicarImpulso(float impulsoHorizontal, float impulsoVertical)
{
    impulsoX = impulsoHorizontal;
    impulsoY = impulsoVertical;
}

// FISICA: impulso con amortiguamiento.
// El golpe le da a Dexter una velocidad de golpe (impulso) que luego
// se va frenando sola: v = v - k * v * dt. Sale rapido y frena suave.
void Dexter::moverPorImpulso(float intervaloTiempo, Mapa &mapa)
{
    if (impulsoX == 0 && impulsoY == 0)
    {
        return;
    }

    // Igual que en mover(): cada eje por separado.
    // Si choca con una pared, el empujon en ese eje se acaba
    float nuevaX = x + impulsoX * intervaloTiempo;
    if (!chocaConPared(nuevaX, y, mapa))
    {
        x = nuevaX;
    }
    else
    {
        impulsoX = 0;
    }

    float nuevaY = y + impulsoY * intervaloTiempo;
    if (!chocaConPared(x, nuevaY, mapa))
    {
        y = nuevaY;
    }
    else
    {
        impulsoY = 0;
    }

    // Amortiguamiento: cada instante pierde una parte de su velocidad,
    // proporcional a la velocidad que lleva (v = v - k * v * dt)
    const float AMORTIGUAMIENTO = 5;
    impulsoX -= impulsoX * AMORTIGUAMIENTO * intervaloTiempo;
    impulsoY -= impulsoY * AMORTIGUAMIENTO * intervaloTiempo;

    // Cuando ya va muy lento, lo dejamos quieto
    if (impulsoX > -10 && impulsoX < 10) impulsoX = 0;
    if (impulsoY > -10 && impulsoY < 10) impulsoY = 0;
}

void Dexter::detenerImpulso()
{
    impulsoX = 0;
    impulsoY = 0;
}