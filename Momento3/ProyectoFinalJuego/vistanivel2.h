#ifndef VISTANIVEL2_H
#define VISTANIVEL2_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QTimer>
#include <QColor>
#include <QGraphicsEllipseItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <vector>
#include "nivel2.h"

class VistaNivel2 : public QGraphicsView
{
    Q_OBJECT
private:
    Nivel2 nivel;
    QGraphicsScene *escena;
    QTimer *temporizador;

    // Posicion donde empieza a dibujarse el mapa dentro de la ventana
    static const int MARGEN_X = 280;
    static const int MARGEN_Y = 48;

    //--------------------------------------------------- Mapa -------------------------------------//
    QGraphicsRectItem *figurasCasillas[Mapa::FILAS][Mapa::COLUMNAS];
    void crearMapa();
    void actualizarMapaGrafico();
    QColor colorCasilla(int tipo);

    //--------------------------------------------------- Dexter -----------------------------------//
    QGraphicsRectItem *figuraDexter;
    void actualizarDexter();

    // Contador de vidas
    QGraphicsTextItem *textoVidas;
    void actualizarContadorVidas();

    //--------------------------------------------------- Bombas -----------------------------------//
    // Una figura por cada bomba del nivel
    std::vector<QGraphicsEllipseItem*> figurasBombas;
    // Una figura por cada casilla con fuego
    std::vector<QGraphicsRectItem*> figurasFuego;
    void actualizarBombasGraficas();
    void actualizarFuegoGrafico();

    //--------------------------------------------------- Enemigos ---------------------------------//
    // Una figura por cada niebla viva
    std::vector<QGraphicsEllipseItem*> figurasNieblas;
    void actualizarNieblasGraficas();

    // Una figura por cada armadillo vivo
    std::vector<QGraphicsRectItem*> figurasArmadillos;
    void actualizarArmadillosGraficos();

    // Una figura por cada mono vivo
    std::vector<QGraphicsRectItem*> figurasMonos;
    void actualizarMonosGraficos();

    //--------------------------------------------------- Power-ups --------------------------------//
    // Una figura por cada power-up y por cada disparo
    std::vector<QGraphicsRectItem*> figurasBeneficios;
    std::vector<QGraphicsEllipseItem*> figurasDisparos;
    void actualizarBeneficiosGraficos();
    void actualizarDisparosGraficos();

    // Muestra cuanto le queda a cada power-up
    QGraphicsTextItem *textoPoderes;
    void actualizarTextoPoderes();

    // Cuenta los cuadros del juego (para hacer parpadear cosas)
    int contadorCuadros;

    //-------------------------------------------------- Interaccion Teclado -------------------------//
    bool arribaPresionada;
    bool abajoPresionada;
    bool izquierdaPresionada;
    bool derechaPresionada;

    void actualizarJuego();

    //--------------------------------------------------- Pausa ----------------------------------//
    bool partidaPausada;
    bool partidaTerminada;
    QGraphicsRectItem *fondoPausa;
    QGraphicsTextItem *textoPausa;

    // Botones del menu de pausa
    QGraphicsProxyWidget *botonContinuar;
    QGraphicsProxyWidget *botonSalirMenu;
    void alternarPausa();
    void finalizarPartida();

public:
    VistaNivel2(bool dificultad = false);

protected:
    void keyPressEvent(QKeyEvent *evento);
    void keyReleaseEvent(QKeyEvent *evento);

signals:
    // Le avisa al MainWindow que hay que volver al menu
    void solicitarMenuPrincipal();
};

#endif // VISTANIVEL2_H