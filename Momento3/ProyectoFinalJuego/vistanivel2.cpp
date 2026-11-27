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

    // Texto de los power-ups, debajo de las vidas
    textoPoderes = escena->addText("");
    textoPoderes->setDefaultTextColor(Qt::white);
    textoPoderes->setFont(QFont("Arial", 14, QFont::Bold));
    textoPoderes->setPos(20, 55);
    textoPoderes->setZValue(20);

    // Detalles de la vista
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFrameShape(QFrame::NoFrame);
    setFixedSize(1280, 720);

    // Fondo del menu de pausa (mismo estilo del nivel 1)
    fondoPausa = escena->addRect(340, 210, 600, 280,
                                 QPen(QColor(140, 170, 112), 3),
                                 QBrush(QColor(38, 59, 52)));
    fondoPausa->setZValue(30);
    fondoPausa->hide();

    textoPausa = escena->addText("PARTIDA PAUSADA");
    textoPausa->setFont(QFont("Consolas", 28, QFont::Bold));
    textoPausa->setDefaultTextColor(QColor(255, 245, 217));
    textoPausa->setPos((1280 - textoPausa->boundingRect().width()) / 2, 230);
    textoPausa->setZValue(31);
    textoPausa->hide();

    // Boton para seguir jugando
    QPushButton *continuar = new QPushButton("CONTINUAR JUGANDO");
    continuar->setFixedSize(360, 50);
    continuar->setFont(QFont("Consolas", 14, QFont::Bold));

    botonContinuar = escena->addWidget(continuar);
    botonContinuar->setPos(460, 315);
    botonContinuar->setZValue(31);
    botonContinuar->hide();

    // Boton para volver al menu
    QPushButton *salir = new QPushButton("SALIR AL MENU PRINCIPAL");
    salir->setFixedSize(360, 50);
    salir->setFont(QFont("Consolas", 14, QFont::Bold));

    botonSalirMenu = escena->addWidget(salir);
    botonSalirMenu->setPos(460, 390);
    botonSalirMenu->setZValue(31);
    botonSalirMenu->hide();

    // Al hacer clic: continuar quita la pausa, salir avisa al MainWindow
    connect(continuar, &QPushButton::clicked, this, &VistaNivel2::alternarPausa);
    connect(salir, &QPushButton::clicked, this, &VistaNivel2::solicitarMenuPrincipal);

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

        // La semilla del mono se dibuja distinto: mas pequena y volando
        if (bombas[i]->esSemillaDelMono())
        {
            figurasBombas[i]->setRect(0, 0, 24, 24);

            // Va desde la casilla del mono hasta la casilla destino
            float progreso = bombas[i]->getProgresoVuelo();
            float origenX = bombas[i]->getColumnaOrigen() * tam;
            float origenY = bombas[i]->getFilaOrigen() * tam;
            float destinoX = bombas[i]->getColumna() * tam;
            float destinoY = bombas[i]->getFila() * tam;

            float semillaX = origenX + (destinoX - origenX) * progreso;
            float semillaY = origenY + (destinoY - origenY) * progreso;

            // Arco: sube en la mitad del vuelo y baja al caer
            semillaY = semillaY - 4 * progreso * (1 - progreso) * 30;

            // +12 para que quede centrada en la casilla
            figurasBombas[i]->setPos(MARGEN_X + semillaX + 12,
                                     MARGEN_Y + semillaY + 12);

            // En el ultimo segundo parpadea en rojo, si no es verde
            bool parpadeoSemilla = bombas[i]->getTiempoMecha() < 1 &&
                                   int(bombas[i]->getTiempoMecha() * 8) % 2 == 0;
            if (parpadeoSemilla)
            {
                figurasBombas[i]->setBrush(QBrush(QColor(200, 30, 30)));
            }
            else
            {
                figurasBombas[i]->setBrush(QBrush(QColor(120, 200, 60)));
            }
            continue;
        }

        // Bomba normal de Dexter
        figurasBombas[i]->setRect(0, 0, tam - 12, tam - 12);
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


void VistaNivel2::actualizarMonosGraficos()
{
    const std::vector<Mono*> &monos = nivel.getMonos();

    // Una figura por mono
    while (figurasMonos.size() < monos.size())
    {
        QGraphicsRectItem *figura = escena->addRect(
            0, 0, monos[0]->getAncho(), monos[0]->getAlto(),
            QPen(Qt::black, 2), QBrush(QColor(30, 110, 50)));
        figura->setZValue(9);
        figurasMonos.push_back(figura);
    }

    while (figurasMonos.size() > monos.size())
    {
        delete figurasMonos.back();
        figurasMonos.pop_back();
    }

    for (int i = 0; i < (int)monos.size(); i++)
    {
        figurasMonos[i]->setPos(MARGEN_X + monos[i]->getX(),
                                MARGEN_Y + monos[i]->getY());
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
    actualizarMonosGraficos();
    actualizarBeneficiosGraficos();
    actualizarDisparosGraficos();
    actualizarContadorVidas();
    actualizarTextoPoderes();

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
        else if (evento->key() == Qt::Key_J) nivel.disparar();
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
    // No se puede pausar una partida que ya termino
    if (partidaTerminada)
    {
        return;
    }

    partidaPausada = !partidaPausada;

    // Soltamos las teclas para que Dexter no siga caminando solo
    arribaPresionada = false;
    abajoPresionada = false;
    izquierdaPresionada = false;
    derechaPresionada = false;

    if (partidaPausada)
    {
        temporizador->stop();
        fondoPausa->show();
        textoPausa->show();
        botonContinuar->show();
        botonSalirMenu->show();
    }
    else
    {
        fondoPausa->hide();
        textoPausa->hide();
        botonContinuar->hide();
        botonSalirMenu->hide();

        // Al hacer clic en el boton, el foco se va al boton;
        // lo devolvemos a la vista para que el teclado funcione
        setFocus();

        temporizador->start(16);
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
    escena->addRect(340, 210, 600, 280,
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
    mensaje->setPos((1280 - mensaje->boundingRect().width()) / 2, 260);
    mensaje->setZValue(31);

    // Boton para volver al menu principal
    QPushButton *regresar = new QPushButton("VOLVER AL MENU PRINCIPAL");
    regresar->setFixedSize(360, 50);
    regresar->setFont(QFont("Consolas", 14, QFont::Bold));

    QGraphicsProxyWidget *botonRegresar = escena->addWidget(regresar);
    botonRegresar->setPos(460, 390);
    botonRegresar->setZValue(31);

    connect(regresar, &QPushButton::clicked, this, &VistaNivel2::solicitarMenuPrincipal);
}

//--------------------------------------------------- Power-ups ----------------------------------//

void VistaNivel2::actualizarBeneficiosGraficos()
{
    const std::vector<Beneficios*> &beneficios = nivel.getBeneficios();

    // Una figura por power-up, igual que con las bombas
    while (figurasBeneficios.size() < beneficios.size())
    {
        QGraphicsRectItem *figura = escena->addRect(
            0, 0, beneficios[0]->getAncho(), beneficios[0]->getAlto(),
            QPen(Qt::white, 2), QBrush(Qt::white));
        figura->setZValue(4);
        figurasBeneficios.push_back(figura);
    }

    while (figurasBeneficios.size() > beneficios.size())
    {
        delete figurasBeneficios.back();
        figurasBeneficios.pop_back();
    }

    for (int i = 0; i < (int)beneficios.size(); i++)
    {
        // Mientras la pared lo tape no se dibuja
        if (beneficios[i]->estaOculto())
        {
            figurasBeneficios[i]->hide();
            continue;
        }

        figurasBeneficios[i]->show();
        figurasBeneficios[i]->setPos(MARGEN_X + beneficios[i]->getX(),
                                     MARGEN_Y + beneficios[i]->getY());

        // Suero en azul claro, pistola en morado
        if (beneficios[i]->getTipo() == Beneficios::SUERO)
        {
            figurasBeneficios[i]->setBrush(QBrush(QColor(60, 200, 230)));
        }
        else
        {
            figurasBeneficios[i]->setBrush(QBrush(QColor(150, 60, 200)));
        }
    }
}

void VistaNivel2::actualizarDisparosGraficos()
{
    const std::vector<DisparoVeneno*> &disparos = nivel.getDisparos();

    while (figurasDisparos.size() < disparos.size())
    {
        QGraphicsEllipseItem *figura = escena->addEllipse(
            0, 0, disparos[0]->getAncho(), disparos[0]->getAlto(),
            QPen(Qt::NoPen), QBrush(QColor(120, 220, 40)));
        figura->setZValue(11);
        figurasDisparos.push_back(figura);
    }

    while (figurasDisparos.size() > disparos.size())
    {
        delete figurasDisparos.back();
        figurasDisparos.pop_back();
    }

    for (int i = 0; i < (int)disparos.size(); i++)
    {
        figurasDisparos[i]->setPos(MARGEN_X + disparos[i]->getX(),
                                   MARGEN_Y + disparos[i]->getY());
    }
}

void VistaNivel2::actualizarTextoPoderes()
{
    Dexter &dexter = nivel.getDexter();
    QString nuevoTexto = "";

    // Solo mostramos los power-ups que estan activos (segundos sin decimales)
    if (dexter.getTiempoSuero() > 0)
    {
        nuevoTexto += "Suero: " + QString::number((int)dexter.getTiempoSuero() + 1) + " s\n";
    }
    if (dexter.tienePistola())
    {
        nuevoTexto += "Pistola (J): " + QString::number((int)dexter.getTiempoPistola() + 1) + " s";
    }

    if (textoPoderes->toPlainText() != nuevoTexto)
    {
        textoPoderes->setPlainText(nuevoTexto);
    }
}