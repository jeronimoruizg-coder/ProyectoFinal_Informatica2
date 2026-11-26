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
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QFont>

using namespace std;

class VistaNivel1 : public QGraphicsView
{
private:
    Nivel1 nivel;
    QGraphicsScene *escena;

    //Representacion Grafica Dexter
    QGraphicsPixmapItem *figuraDexter;

    //Sprites de dexter para animacion
    vector<QPixmap> spritesReposo;
    vector<QPixmap> spritesCaminata;
    vector<QPixmap> spritesSalto;
    vector<QPixmap> spritesRecoleccion;
    vector<QPixmap> spritesDano;

    // Representaciones graficamente de las muestras
    vector<QGraphicsPixmapItem*> figurasMuestras;
    // Texto que muestra la cantidad recolectada
    QGraphicsTextItem *textoMuestras;
    // Ultima cantidad mostrada en pantalla
    int muestrasMostradas;
    // Funciones para administrar el contador
    void crearContadorMuestras();
    void actualizarContadorMuestras();

    // Actualiza la representacion grafica de las muestras
    void actualizarMuestrasGraficas();

    // Imagenes de las muestras botanicas
    vector<QPixmap> spritesMuestrasComunes;
    QPixmap spriteMuestraEspecial;
    // Carga las imagenes de las muestras una sola vez
    void cargarSpritesMuestras();

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
