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
#include <QGraphicsColorizeEffect>

using namespace std;

class VistaNivel1 : public QGraphicsView
{
private:
    Nivel1 nivel;
    QGraphicsScene *escena;
//--------------------------------------------------- Dexter -----------------------------------//
    //Representacion Grafica Dexter
    QGraphicsPixmapItem *figuraDexter;

    //Sprites de dexter para animacion
    vector<QPixmap> spritesReposo;
    vector<QPixmap> spritesCaminata;
    vector<QPixmap> spritesSalto;
    vector<QPixmap> spritesRecoleccion;

    // Efecto visual de daño sobre Dexter
    QGraphicsColorizeEffect *efectoDano;
    // Actualizamos el parpadeo rojo
    void actualizarEfectoDano();

    // Contador de vidas
    QGraphicsTextItem *textoVidas;
    int vidasMostradas;

    // Sprite de muerte
    QPixmap spriteMuerte;

    // Control de finalizacion de la partida
    bool partidaTerminada;

    // Funciones del contador de vidas
    void crearContadorVidas();
    void actualizarContadorVidas();

    // Funciones relacionadas con la derrota
    void cargarSpriteMuerte();
    void finalizarPartida();
//--------------------------------------------------- Muestras ----------------------------------//
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
//-------------------------------------------------- Interaccion Teclado -------------------------//
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

//--------------------------------------------------- Semilla ----------------------------------//
    // Sprite de la semilla explosiva
    QPixmap spriteSemilla;
    // Fotogramas de la explosion
    vector<QPixmap> spritesExplosion;
    // Representaciones graficas de las semillas
    vector<QGraphicsPixmapItem*> figurasSemillas;
    // Funciones graficas de las semillas
    void cargarSpriteSemilla();
    void cargarSpritesExplosion();
    void actualizarSemillasGraficas();
//--------------------------------------------------- Beneficios ----------------------------------//
    // Sprites de los objetos beneficiosos
    QPixmap spriteMedicinal;
    QPixmap spriteSuero;

    // Representaciones graficas de los beneficios
    vector<QGraphicsPixmapItem*> figurasBeneficios;

    // Funciones graficas de los beneficios
    void cargarSpritesBeneficios();
    void actualizarBeneficiosGraficos();
//--------------------------------------------------- Hormigas ----------------------------------//
    // Fotogramas de la hormiga
    vector<QPixmap> spritesHormiga;

    // Figuras graficas de las hormigas
    vector<QGraphicsPixmapItem*> figurasHormigas;

    // Control de la animacion
    int fotogramaHormiga;
    int tiempoAnimacionHormiga;

    // Funciones graficas
    void cargarSpritesHormiga();
    void actualizarHormigasGraficas();
//--------------------------------------------------- Jabalis ----------------------------------//
    // Fotogramas del jabali y su advertencia
    vector<QPixmap> spritesJabali;
    vector<QPixmap> spritesAdvertencia;

    // Representaciones graficas
    vector<QGraphicsPixmapItem*> figurasJabalis;

    QGraphicsPixmapItem *figuraAdvertencia;

    // Control de la animacion del jabali
    int fotogramaJabali;
    int tiempoAnimacionJabali;

    // Control de la animacion de advertencia
    int fotogramaAdvertencia;
    int tiempoAnimacionAdvertencia;

    // Funciones graficas
    void cargarSpritesJabali();
    void actualizarJabalisGraficos();
    void actualizarAdvertenciaJabali();
//--------------------------------------------------- Guacamayas ----------------------------------//
    // Fotogramas compartidos de la guacamaya
    vector<QPixmap> spritesGuacamaya;

    // Representaciones graficas
    vector<QGraphicsPixmapItem*> figurasGuacamayas;

    // Control de la animacion
    int fotogramaGuacamaya;
    int tiempoAnimacionGuacamaya;

    // Funciones graficas
    void cargarSpritesGuacamaya();
    void actualizarGuacamayasGraficas();
//--------------------------------------------------- Excrementos ----------------------------------//
    // Sprites compartidos de los excrementos
    QPixmap spriteExcrementoCaida;
    QPixmap spriteExcrementoSuelo;

    // Representaciones graficas
    vector<QGraphicsPixmapItem*> figurasExcrementos;

    // Funciones graficas
    void cargarSpritesExcrementos();
    void actualizarExcrementosGraficos();



public:
    VistaNivel1();

protected:
    void keyPressEvent(QKeyEvent *evento);
    void keyReleaseEvent(QKeyEvent *evento);

};

#endif // VISTANIVEL1_H
