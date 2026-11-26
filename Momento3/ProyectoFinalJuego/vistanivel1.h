#ifndef VISTANIVEL1_H
#define VISTANIVEL1_H

#include <QGraphicsView> //Interfaz grafica
#include <QGraphicsScene>// Escena
#include <QGraphicsPixmapItem>//Objetos que se ven en la pantalla
#include <QKeyEvent>//Eventos del teclado
#include <QTimer>//temporizador de actualizacion
#include "nivel1.h"
#include <vector>
#include <QPixmap>
#include <QString>

using namespace std;

class VistaNivel1 : public QGraphicsView
{
private:
    Nivel1 nivel;
    QGraphicsScene *escena;
    QGraphicsPixmapItem *figuraDexter;
    vector<QPixmap> spritesReposo;
    vector<QPixmap> spritesCaminata;
    vector<QPixmap> spritesSalto;
    vector<QPixmap> spritesRecoleccion;
    vector<QPixmap> spritesDano;

    bool izquierdaPresionada; // comprobamos que tecla se esta presionando
    bool derechaPresionada;
    bool saltarPresionado;

    int fotogramaActual;//variables para animacion de sprites
    int tiempoAnimacion;
    int estadoAnimacion;
    bool mirandoDerecha;
    void cargarSprites();
    void cargarAnimacion(vector<QPixmap> &sprites, QString ruta, int cantidad);

    QTimer *temporizador;
    void actualizarJuego();
    void actualizarAnimacion(int direccion);
    void mostrarFotograma(const QPixmap &imagen);// Muestra el fotograma segun la orientacion de Dexter
public:
    VistaNivel1();

protected:
    void keyPressEvent(QKeyEvent *evento);
    void keyReleaseEvent(QKeyEvent *evento);

};

#endif // VISTANIVEL1_H
