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
    partidaTerminada = false;
    contadorCuadros = 0;

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

// Repinta las casillas que cambiaron (paredes rotas o puerta abierta)
void VistaNivel2::actualizarMapaGrafico()
{
    Mapa &mapa = nivel.getMapa();

    for (int f = 0; f < Mapa::FILAS; f++)
    {
        for (int c = 0; c < Mapa::COLUMNAS; c++)
        {
            QColor nuevoColor = colorCasilla(mapa.getCasilla(f, c));

            // Solo cambiamos el color si es distinto al actual
            if (figurasCasillas[f][c]->brush().color() != nuevoColor)
            {
                figurasCasillas[f][c]->setBrush(QBrush(nuevoColor));
            }
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

//--------------------------------------------------- Bombas -----------------------------------//

void VistaNivel2::actualizarBombasGraficas()
{
    const std::vector<Bomba*> &bombas = nivel.getBombas();
    int tam = Mapa::TAM_CASILLA;

    // Creamos figuras si hay mas bombas que figuras
    while (figurasBombas.size() < bombas.size())
    {
        QGraphicsEllipseItem *figura = escena->addEllipse(
            0, 0, tam - 12, tam - 12, QPen(Qt::black), QBrush(QColor(20, 20, 20)));
        figura->setZValue(5);
        figurasBombas.push_back(figura);
    }

    // Borramos las figuras que sobran
    while (figurasBombas.size() > bombas.size())
    {
        delete figurasBombas.back();
        figurasBombas.pop_back();
    }

    for (int i = 0; i < (int)bombas.size(); i++)
    {
        // Cuando explota, la bomba ya no se dibuja (se ve el fuego)
        if (bombas[i]->haExplotado())
        {
            figurasBombas[i]->hide();
            continue;
        }

        figurasBombas[i]->show();
        figurasBombas[i]->setPos(MARGEN_X + bombas[i]->getX() + 6,
                                 MARGEN_Y + bombas[i]->getY() + 6);

        // En el ultimo segundo parpadea en rojo para avisar
        bool parpadeo = bombas[i]->getTiempoMecha() < 1 &&
                        int(bombas[i]->getTiempoMecha() * 8) % 2 == 0;

        if (parpadeo)
        {
            figurasBombas[i]->setBrush(QBrush(QColor(200, 30, 30)));
        }
        else
        {
            figurasBombas[i]->setBrush(QBrush(QColor(20, 20, 20)));
        }
    }
}

void VistaNivel2::actualizarFuegoGrafico()
{
    const std::vector<Bomba*> &bombas = nivel.getBombas();
    int tam = Mapa::TAM_CASILLA;

    // Contamos cuantas casillas con fuego hay en total
    int totalFuego = 0;
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        totalFuego += bombas[i]->getCantidadFuego();
    }

    // Ajustamos la cantidad de figuras de fuego
    while ((int)figurasFuego.size() < totalFuego)
    {
        QGraphicsRectItem *figura = escena->addRect(
            0, 0, tam, tam, QPen(Qt::NoPen), QBrush(QColor(255, 140, 0, 200)));
        figura->setZValue(6);
        figurasFuego.push_back(figura);
    }

    while ((int)figurasFuego.size() > totalFuego)
    {
        delete figurasFuego.back();
        figurasFuego.pop_back();
    }

    // Ponemos cada figura en su casilla
    int indice = 0;
    for (int i = 0; i < (int)bombas.size(); i++)
    {
        for (int k = 0; k < bombas[i]->getCantidadFuego(); k++)
        {
            figurasFuego[indice]->setPos(MARGEN_X + bombas[i]->getColumnaFuego(k) * tam,
                                         MARGEN_Y + bombas[i]->getFilaFuego(k) * tam);
            indice++;
        }
    }
}

//--------------------------------------------------- Enemigos ---------------------------------//

void VistaNivel2::actualizarNieblasGraficas()
{
    const std::vector<Niebla*> &nieblas = nivel.getNieblas();

    // Mismo truco que con las bombas: una figura por niebla
    while (figurasNieblas.size() < nieblas.size())
    {
        QGraphicsEllipseItem *figura = escena->addEllipse(
            0, 0, nieblas[0]->getAncho(), nieblas[0]->getAlto(),
            QPen(Qt::NoPen), QBrush(QColor(230, 230, 255, 170)));
        figura->setZValue(8);
        figurasNieblas.push_back(figura);
    }

    while (figurasNieblas.size() > nieblas.size())
    {
        delete figurasNieblas.back();
        figurasNieblas.pop_back();
    }

    for (int i = 0; i < (int)nieblas.size(); i++)
    {
        figurasNieblas[i]->setPos(MARGEN_X + nieblas[i]->getX(),
                                  MARGEN_Y + nieblas[i]->getY());
    }
}

void VistaNivel2::actualizarArmadillosGraficos()
{
    const std::vector<Armadillo*> &armadillos = nivel.getArmadillos();

    // Una figura por armadillo
    while (figurasArmadillos.size() < armadillos.size())
    {
        QGraphicsRectItem *figura = escena->addRect(
            0, 0, armadillos[0]->getAncho(), armadillos[0]->getAlto(),
            QPen(Qt::black, 2), QBrush(QColor(110, 75, 40)));
        figura->setZValue(9);
        figurasArmadillos.push_back(figura);
    }

    while (figurasArmadillos.size() > armadillos.size())
    {
        delete figurasArmadillos.back();
        figurasArmadillos.pop_back();
    }

    for (int i = 0; i < (int)armadillos.size(); i++)
    {
        figurasArmadillos[i]->setPos(MARGEN_X + armadillos[i]->getX(),
                                     MARGEN_Y + armadillos[i]->getY());

        // El color avisa en que estado esta
        QColor color(110, 75, 40);   // caminando

        if (armadillos[i]->estaPreparando())
        {
            // Parpadea en amarillo antes de embestir
            if ((contadorCuadros / 5) % 2 == 0)
            {
                color = QColor(240, 200, 40);
            }
        }
        else if (armadillos[i]->estaEmbistiendo())
        {
            color = QColor(200, 60, 30);
        }
        else if (armadillos[i]->estaAturdido())
        {
            color = QColor(150, 150, 150);
        }

        figurasArmadillos[i]->setBrush(QBrush(color));
    }
}

//-------------------------------------------------- Juego y teclado -------------------------//

void VistaNivel2::actualizarJuego()
{
    if (partidaTerminada)
    {
        return;
    }

        contadorCuadros++;

    // Convertimos las teclas presionadas en una direccion
    int dirX = 0;
    int dirY = 0;

    if (izquierdaPresionada) dirX--;
    if (derechaPresionada) dirX++;
    if (arribaPresionada) dirY--;
    if (abajoPresionada) dirY++;

    // La logica avanza y luego dibujamos lo que quedo
    nivel.actualizar(dirX, dirY, 0.016);

    actualizarMapaGrafico();
    actualizarDexter();
    actualizarBombasGraficas();
    actualizarFuegoGrafico();
    actualizarNieblasGraficas();
    actualizarArmadillosGraficos();
    actualizarContadorVidas();

    if (nivel.haPerdido() || nivel.haGanado())
    {
        finalizarPartida();
    }
}

void VistaNivel2::keyPressEvent(QKeyEvent *evento)
{
    // Ignoramos las repeticiones automaticas del teclado
    if (evento->isAutoRepeat() || partidaTerminada)
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
    else if (evento->key() == Qt::Key_Space) nivel.ponerBomba();
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

void VistaNivel2::finalizarPartida()
{
    if (partidaTerminada)
    {
        return;
    }

    partidaTerminada = true;
    temporizador->stop();

    // Mismo estilo de mensaje que el nivel 1
    escena->addRect(340, 260, 600, 180,
                    QPen(QColor(140, 170, 112), 3),
                    QBrush(QColor(38, 59, 52)))->setZValue(30);

    // El mensaje depende de como termino la partida
    QString resultado = "¡HAS PERDIDO!";
    if (nivel.haGanado())
    {
        resultado = "¡VICTORIA!";
    }

    QGraphicsTextItem *mensaje = escena->addText(resultado);
    mensaje->setFont(QFont("Consolas", 28, QFont::Bold));
    mensaje->setDefaultTextColor(QColor(255, 245, 217));
    mensaje->setPos((1280 - mensaje->boundingRect().width()) / 2,
                    (700 - mensaje->boundingRect().height()) / 2);
    mensaje->setZValue(31);
}