#include "vistanivel2.h"
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QString>

VistaNivel2::VistaNivel2(bool dificultad): nivel(dificultad)
{
    arribaPresionada = false;
    abajoPresionada = false;
    izquierdaPresionada = false;
    derechaPresionada = false;
    partidaPausada = false;

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1280, 720);
    setScene(escena);
    escena->setBackgroundBrush(QBrush(QColor(30, 30, 40)));

    crearMapa();

    // Por ahora Dexter es un cuadro azul
    Dexter &dexter = nivel.getDexter();
    figuraDexter = escena->addRect(0, 0, dexter.getAncho(), dexter.getAlto(),
                                   QPen(Qt::black), QBrush(QColor(40, 110, 220)));
    figuraDexter->setZValue(10);
    actualizarDexter();

    // Contador de vidas
    textoVidas = escena->addText("");
    textoVidas->setDefaultTextColor(Qt::white);
    textoVidas->setFont(QFont("Arial", 18, QFont::Bold));
    textoVidas->setPos(20, 15);
    textoVidas->setZValue(20);
    actualizarContadorVidas();

    // Detalles de la vista
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setFixedSize(1280, 720);

    // Mensaje de pausa
    fondoPausa = escena->addRect(340, 260, 600, 180,
                                 QPen(QColor(140, 170, 112), 3),
                                 QBrush(QColor(38, 59, 52)));
    fondoPausa->setZValue(30);
    fondoPausa->hide();

    textoPausa = escena->addText("PARTIDA PAUSADA\nESC para continuar");
    textoPausa->setFont(QFont("Consolas", 24, QFont::Bold));
    textoPausa->setDefaultTextColor(QColor(255, 245, 217));
    textoPausa->setPos((1280 - textoPausa->boundingRect().width()) / 2,
                       (700 - textoPausa->boundingRect().height()) / 2);
    textoPausa->setZValue(31);
    textoPausa->hide();

    // Temporizador del juego
    temporizador = new QTimer(this);
    connect(temporizador, &QTimer::timeout, this, &VistaNivel2::actualizarJuego);
    temporizador->start(16);
}

//--------------------------------------------------- Mapa -------------------------------------//

void VistaNivel2::crearMapa()
{
    int tam = Mapa::TAM_CASILLA;
    Mapa &mapa = nivel.getMapa();

    for (int f = 0; f < Mapa::FILAS; f++)
    {
        for (int c = 0; c < Mapa::COLUMNAS; c++)
        {
            figurasCasillas[f][c] = escena->addRect(
                MARGEN_X + c * tam, MARGEN_Y + f * tam, tam, tam,
                QPen(QColor(0, 0, 0, 60)),
                QBrush(colorCasilla(mapa.getCasilla(f, c))));
        }
    }
}

QColor VistaNivel2::colorCasilla(int tipo)
{
    if (tipo == Mapa::PARED_FIJA)
    {
        return QColor(70, 70, 70);
    }
    if (tipo == Mapa::PARED_ROMPIBLE)
    {
        return QColor(150, 100, 50);
    }
    if (tipo == Mapa::PUERTA)
    {
        if (nivel.getMapa().estaPuertaAbierta())
        {
            return QColor(40, 170, 60);
        }
        return QColor(200, 40, 40);
    }
    return QColor(200, 200, 200);   // pasillo
}

//--------------------------------------------------- Dexter -----------------------------------//

void VistaNivel2::actualizarDexter()
{
    Dexter &dexter = nivel.getDexter();

    figuraDexter->setPos(MARGEN_X + dexter.getX(), MARGEN_Y + dexter.getY());

    // Parpadea en rojo mientras es invulnerable
    if (dexter.debeTitilarRojo())
    {
        figuraDexter->setBrush(QBrush(QColor(220, 40, 40)));
    }
    else
    {
        figuraDexter->setBrush(QBrush(QColor(40, 110, 220)));
    }
}

void VistaNivel2::actualizarContadorVidas()
{
    QString nuevoTexto = "Vidas: " + QString::number(nivel.getDexter().getVidas());

    // Solo cambiamos el texto cuando cambia el numero
    if (textoVidas->toPlainText() != nuevoTexto)
    {
        textoVidas->setPlainText(nuevoTexto);
    }
}

//-------------------------------------------------- Juego y teclado -------------------------//

void VistaNivel2::actualizarJuego()
{
    int dirX = 0;
    int dirY = 0;

    if (izquierdaPresionada) dirX--;
    if (derechaPresionada) dirX++;
    if (arribaPresionada) dirY--;
    if (abajoPresionada) dirY++;

    nivel.actualizar(dirX, dirY, 0.016);

    actualizarDexter();
    actualizarContadorVidas();
}

void VistaNivel2::keyPressEvent(QKeyEvent *evento)
{
    // Ignoramos las repeticiones automaticas del teclado
    if (evento->isAutoRepeat())
    {
        return;
    }

    if (evento->key() == Qt::Key_Escape)
    {
        alternarPausa();
        return;
    }

    if (partidaPausada)
    {
        return;
    }

    if (evento->key() == Qt::Key_W) arribaPresionada = true;
    else if (evento->key() == Qt::Key_S) abajoPresionada = true;
    else if (evento->key() == Qt::Key_A) izquierdaPresionada = true;
    else if (evento->key() == Qt::Key_D) derechaPresionada = true;
    else QGraphicsView::keyPressEvent(evento);
}

void VistaNivel2::keyReleaseEvent(QKeyEvent *evento)
{
    if (evento->isAutoRepeat())
    {
        return;
    }

    if (evento->key() == Qt::Key_W) arribaPresionada = false;
    else if (evento->key() == Qt::Key_S) abajoPresionada = false;
    else if (evento->key() == Qt::Key_A) izquierdaPresionada = false;
    else if (evento->key() == Qt::Key_D) derechaPresionada = false;
    else QGraphicsView::keyReleaseEvent(evento);
}

//--------------------------------------------------- Pausa ----------------------------------//

void VistaNivel2::alternarPausa()
{
    partidaPausada = !partidaPausada;

    if (partidaPausada)
    {
        temporizador->stop();
        fondoPausa->show();
        textoPausa->show();
    }
    else
    {
        // Soltamos las teclas para que Dexter no siga caminando solo
        arribaPresionada = false;
        abajoPresionada = false;
        izquierdaPresionada = false;
        derechaPresionada = false;

        temporizador->start(16);
        fondoPausa->hide();
        textoPausa->hide();
    }
}