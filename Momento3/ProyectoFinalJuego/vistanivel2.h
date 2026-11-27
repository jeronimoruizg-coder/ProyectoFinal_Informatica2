#ifndef VISTANIVEL2_H
#define VISTANIVEL2_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>
#include <QTimer>
#include <QColor>
#include "nivel2.h"

class VistaNivel2 : public QGraphicsView
{
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
    QColor colorCasilla(int tipo);

    //--------------------------------------------------- Dexter -----------------------------------//
    QGraphicsRectItem *figuraDexter;
    void actualizarDexter();

    // Contador de vidas
    QGraphicsTextItem *textoVidas;
    void actualizarContadorVidas();

    //-------------------------------------------------- Interaccion Teclado -------------------------//
    bool arribaPresionada;
    bool abajoPresionada;
    bool izquierdaPresionada;
    bool derechaPresionada;

    void actualizarJuego();

    //--------------------------------------------------- Pausa ----------------------------------//
    bool partidaPausada;
    QGraphicsRectItem *fondoPausa;
    QGraphicsTextItem *textoPausa;
    void alternarPausa();

public:
    VistaNivel2(bool dificultad = false);

protected:
    void keyPressEvent(QKeyEvent *evento);
    void keyReleaseEvent(QKeyEvent *evento);
};

#endif // VISTANIVEL2_H