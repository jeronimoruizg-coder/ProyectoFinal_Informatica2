#ifndef VISTANIVEL1_H
#define VISTANIVEL1_H

#include <QGraphicsView> //Interfaz grafica
#include <QGraphicsScene>// Escena
#include <QGraphicsPixmapItem>//Objetos que se ven en la pantalla
#include <QKeyEvent>//Eventos del teclado

class VistaNivel1 : public QGraphicsView
{
private:
    QGraphicsScene *escena;
    QGraphicsPixmapItem *figuraDexter;
    bool izquierdaPresionada; // comprobamos que tecla se esta presionando
    bool derechaPresionada;
public:
    VistaNivel1();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

};

#endif // VISTANIVEL1_H
