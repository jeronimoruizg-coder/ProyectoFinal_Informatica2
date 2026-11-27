#ifndef BOMBA_H
#define BOMBA_H

#include "objetos.h"
#include "mapa.h"
#include <vector>

class Bomba : public Objetos
{
private:
    // Casilla del mapa donde se puso la bomba
    int fila;
    int columna;

    // Segundos que faltan para que explote
    float tiempoMecha;

    // Cuantas casillas avanza el fuego hacia cada lado
    int alcance;

    // Dexter esta encima de la bomba al ponerla; mientras no salga,
    // la bomba no lo bloquea
    bool dexterEncima;

    // Etapa de explosion (igual que la semilla del nivel 1)
    bool explotada;
    float tiempoExplosion;

    // Casillas a las que llego el fuego.
    // La posicion i de los dos vectores forma una casilla
    std::vector<int> filasFuego;
    std::vector<int> columnasFuego;


    // Datos de la semilla explosiva que lanza el mono
    bool esSemilla;
    int filaOrigen, columnaOrigen;   // desde donde la lanzaron
    int opcionUsada;                 // estrategia de la inteligencia

    void agregarFuego(int filaFuego, int columnaFuego);

public:
    Bomba(int filaInicial, int columnaInicial);

    // Tiempos de la semilla del mono
    static constexpr float DURACION_VUELO = 0.5;
    static constexpr float TIEMPO_EN_SUELO = 2.5;

    // Constructor para la semilla del mono (sobrecarga de constructores)
    Bomba(int filaDestino, int columnaDestino, int filaLanzamiento,
          int columnaLanzamiento, int opcion);

    // Metodo heredado de Objetos
    void actualizar() override;
    void actualizar(float intervaloTiempo);

    // Etapas de la bomba
    bool debeExplotar() const;
    void explotar(Mapa &mapa);
    bool haExplotado() const;
    bool terminoExplosion() const;
    int getFotogramaExplosion() const;

    // Consultas del fuego
    bool hayFuegoEn(int filaConsulta, int columnaConsulta) const;
    int getCantidadFuego() const;
    int getFilaFuego(int indice) const;
    int getColumnaFuego(int indice) const;

    int getFila() const;
    int getColumna() const;
    float getTiempoMecha() const;

    // La bomba es solida mientras no explote y Dexter ya se haya bajado
    bool esSolidaParaDexter() const;
    bool estaDexterEncima() const;
    void dexterSeBajo();

    int getAlcance() const;

    // Consultas de la semilla del mono
    bool esSemillaDelMono() const;
    int getOpcionUsada() const;
    int getFilaOrigen() const;
    int getColumnaOrigen() const;
    float getProgresoVuelo() const;
};

#endif // BOMBA_H