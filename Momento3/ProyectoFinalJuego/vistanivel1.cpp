#include "vistanivel1.h"
#include <QPixmap>
#include <QColor>
#include <QBrush>
#include <QPen>

VistaNivel1::VistaNivel1() {
    izquierdaPresionada = false;
    derechaPresionada = false;
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1280, 720); // Le da las dimensiones a la escena, en este caso 1280x720
    setScene(escena); // Esta funcion sirve para que la vista muestre la escena actual
    escena->setBackgroundBrush(QBrush(QColor(180, 217, 235))); // le damos color al fondo de la escena base

    // Creamos el suelo de la escena:
    escena->addRect(0, 650, 1280, 70, //Le damos las dimensiones al suelo
                    QPen(Qt::NoPen), // Lo creamos sin bordes
                    QBrush(QColor(135, 96, 68))); // Rellenamos el rectanguno(suelo) con el color

    //Agregamos el objeto de dexter
    QPixmap imagenDexter(50, 80); //Creamos una imagen de 50pixeles de ancho x 80 de alto
    imagenDexter.fill(Qt::blue); // Agregamos el relleno a la imagen
    figuraDexter = escena->addPixmap(imagenDexter); //Se crea un objeto QGraphicsPixmapItem y se guarda su puntero
    figuraDexter->setPos(200, 570);//Le damos una posicion inicial a Dexter

    //Detalles de la vista grafica
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//Desactiva el desplazamiento Horizontal
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// Desactiva el desplazamiento Vertical
    setFrameShape(QFrame::NoFrame);// Elimina el borde que Qt coloca alrededor de la vista.
    setFixedSize(1280, 720);// Le damos el tamaño fijo a nuestra Vista grafica, lo que se va a ver al final

}

// Detectamos cuando el jugador presiona una tecla
void VistaNivel1::keyPressEvent(QKeyEvent *event)
{
    // Ignoramos las repeticiones automaticas del teclado
    if (event->isAutoRepeat())
    {
        return;
    }

    // Detectamos el movimiento hacia la izquierda
    if (event->key() == Qt::Key_A ||
        event->key() == Qt::Key_Left)
    {
        izquierdaPresionada = true;
    }

    // Detectamos el movimiento hacia la derecha
    else if (event->key() == Qt::Key_D ||
             event->key() == Qt::Key_Right)
    {
        derechaPresionada = true;
    }

    else
    {
        QGraphicsView::keyPressEvent(event);
    }
}

// Detectamos cuando el jugador suelta una tecla
void VistaNivel1::keyReleaseEvent(QKeyEvent *event)
{
    // Ignoramos las repeticiones automaticas del teclado
    if (event->isAutoRepeat())
    {
        return;
    }

    // Dejamos de solicitar movimiento hacia la izquierda
    if (event->key() == Qt::Key_A ||
        event->key() == Qt::Key_Left)
    {
        izquierdaPresionada = false;
    }

    // Dejamos de solicitar movimiento hacia la derecha
    else if (event->key() == Qt::Key_D ||
             event->key() == Qt::Key_Right)
    {
        derechaPresionada = false;
    }

    else
    {
        QGraphicsView::keyReleaseEvent(event);
    }
}