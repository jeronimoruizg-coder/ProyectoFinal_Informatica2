#include "vistanivel1.h"
#include <QPixmap>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QTransform>
#include <QString>
#include <stdexcept>
#include <QMessageBox>
#include <QDir>
#include <QStringList>
#include <QDebug>

VistaNivel1::VistaNivel1(bool dificultad): nivel(dificultad)
{
    partidaTerminada = false;
    partidaPausada = false;

    //Manejo de la interaccion con teclado
    izquierdaPresionada = false;
    derechaPresionada = false;
    saltarPresionado = false;

    //Manejo de animacion sprites
    fotogramaActual = 0;
    tiempoAnimacion = 0;
    estadoAnimacion = 0;
    mirandoDerecha = true;
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1280, 720); // Le da las dimensiones a la escena, en este caso 1280x720
    setScene(escena); // Esta funcion sirve para que la vista muestre la escena actual
    escena->setBackgroundBrush(QBrush(QColor(180, 217, 235))); // le damos color al fondo de la escena base

    // Creamos el suelo de la escena:
    escena->addRect(0, 650, 1280, 70, //Le damos las dimensiones al suelo
                    QPen(Qt::NoPen), // Lo creamos sin bordes
                    QBrush(QColor(135, 96, 68))); // Rellenamos el rectanguno(suelo) con el color

    //Agregamos el objeto de dexter
    // Cargamos todos los sprites una sola vez
    // Intentamos cargar todos los sprites del personaje
    try
    {
        cargarSprites();
        cargarSpritesMuestras();
        cargarSpriteSemilla();
        cargarSpritesExplosion();
        cargarSpriteMuerte();
        cargarSpritesBeneficios();
        cargarSpritesHormiga();
        cargarSpritesJabali();
        cargarSpritesGuacamaya();
        cargarSpritesExcrementos();
    }
    catch (const runtime_error &error)
    {
        // Informamos que recurso produjo el error
        QMessageBox::critical(this, "Error de recursos",
                              error.what());

        // Detenemos la inicializacion del nivel
        return;
    }


    // Creamos una unica figura para la advertencia
    figuraAdvertencia =
        escena->addPixmap(spritesAdvertencia[0]);
    // La colocamos por encima de los objetos del juego
    figuraAdvertencia->setZValue(15);
    // Inicialmente permanece oculta
    figuraAdvertencia->hide();



    // Mostramos el fotograma de reposo Dexter
    figuraDexter = escena->addPixmap(spritesReposo[0]);
    figuraDexter->setScale(2);  // Duplicamos el tamaño visual de la imagen
    figuraDexter->setOffset(-19.5, -25); // Ajustamos la imagen respecto a la posicion fisica
    figuraDexter->setPos(nivel.getDexter().getX(), nivel.getDexter().getY());//Le damos una posicion inicial a Dexter

    // Creamos el efecto visual de daño
    efectoDano = new QGraphicsColorizeEffect();
    // Configuramos el color rojo
    efectoDano->setColor(Qt::red);
    // Intensidad del efecto
    efectoDano->setStrength(0.8);
    // Asociamos el efecto al sprite de Dexter
    figuraDexter->setGraphicsEffect(efectoDano);
    // Inicialmente permanece desactivado
    efectoDano->setEnabled(false);

    // Creamos el contador de Flores recolectadas
    crearContadorMuestras();

    // Creamos el contador de tiempo
    textoTiempo = escena->addText("Tiempo: 90 s");
    textoTiempo->setDefaultTextColor(Qt::white);
    textoTiempo->setFont(QFont("Arial", 18, QFont::Bold));
    textoTiempo->setPos(550, 15);
    textoTiempo->setZValue(20);

    //Creamos el contador de vidas
    crearContadorVidas();

    //Detalles de la vista grafica
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//Desactiva el desplazamiento Horizontal
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// Desactiva el desplazamiento Vertical
    setFrameShape(QFrame::NoFrame);// Elimina el borde que Qt coloca alrededor de la vista.
    setFixedSize(1280, 720);// Le damos el tamaño fijo a nuestra Vista grafica, lo que se va a ver al final

    // Creamos el fondo de pausa
    fondoPausa = escena->addRect(
        340, 210, 600, 280,
        QPen(QColor(140, 170, 112), 3),
        QBrush(QColor(38, 59, 52))
        );

    fondoPausa->setZValue(30);
    fondoPausa->hide();
    // Creamos el titulo de pausa
    textoPausa = escena->addText("PARTIDA PAUSADA");
    textoPausa->setFont(
        QFont("Consolas", 28, QFont::Bold));
    textoPausa->setDefaultTextColor(
        QColor(255, 245, 217));
    textoPausa->setPos(
        (1280 - textoPausa->boundingRect().width()) / 2,
        230
        );
    textoPausa->setZValue(31);
    textoPausa->hide();
    // Creamos el boton continuar
    QPushButton *continuar =
        new QPushButton("CONTINUAR JUGANDO");
    continuar->setFixedSize(360, 50);
    continuar->setFont(
        QFont("Consolas", 14, QFont::Bold));

    // Agregamos el boton a la escena
    botonContinuar = escena->addWidget(continuar);
    botonContinuar->setPos(460, 315);
    botonContinuar->setZValue(31);
    botonContinuar->hide();

    // Creamos el boton para salir al menu
    QPushButton *salir =
        new QPushButton("SALIR AL MENU PRINCIPAL");
    salir->setFixedSize(360, 50);
    salir->setFont(
        QFont("Consolas", 14, QFont::Bold));

    // Agregamos el boton a la escena
    botonSalirMenu = escena->addWidget(salir);
    botonSalirMenu->setPos(460, 390);
    botonSalirMenu->setZValue(31);
    botonSalirMenu->hide();

    // Conectamos los botones
    connect(continuar, &QPushButton::clicked,
            this, &VistaNivel1::alternarPausa);
    connect(salir, &QPushButton::clicked,
            this, &VistaNivel1::solicitarMenuPrincipal);

    // Creamos el temporizador
    temporizador = new QTimer(this);
    // Conectamos la señal con nuestra funcion
    connect(temporizador, &QTimer::timeout,this, &VistaNivel1::actualizarJuego);
    temporizador->start(16);

    // inicializamos los fotogramas de hormiga
    fotogramaHormiga = 0;
    tiempoAnimacionHormiga = 0;

    // inicializamos los fotogramas de jabali y advertencia
    fotogramaJabali = 0;
    tiempoAnimacionJabali = 0;

    fotogramaAdvertencia = 0;
    tiempoAnimacionAdvertencia = 0;

    // inicializamos los fotogramas de Guacamaya
    fotogramaGuacamaya = 0;
    tiempoAnimacionGuacamaya = 0;

}

// Detectamos cuando el jugador presiona una tecla
void VistaNivel1::keyPressEvent(QKeyEvent *evento)
{
    if (partidaTerminada)
    {
        return;
    }

    // Ignoramos las repeticiones automaticas del teclado
    if (evento->isAutoRepeat())
    {
        return;
    }

    // Pausamos o reanudamos con Escape
    if (evento->key() == Qt::Key_Escape)
    {
        alternarPausa();
        return;
    }

    // Impedimos acciones durante la pausa
    if (partidaPausada || partidaTerminada)
    {
        return;
    }

    // Detectamos el movimiento hacia la izquierda
    if (evento->key() == Qt::Key_A)
    {
        izquierdaPresionada = true;
    }

    // Detectamos el movimiento hacia la derecha
    else if (evento->key() == Qt::Key_D)
    {
        derechaPresionada = true;
    }
    //Detectamos el salto
    else if (evento->key() == Qt::Key_W)
    {
        saltarPresionado = true;
    }

    // Detectamos la accion de utilizar la red
    else if (evento->key() == Qt::Key_Space)
    {
        nivel.recolectar();
    }

    else
    {
        QGraphicsView::keyPressEvent(evento);
    }
}

// Detectamos cuando el jugador suelta una tecla
void VistaNivel1::keyReleaseEvent(QKeyEvent *evento)
{
    if (partidaTerminada)
    {
        return;
    }

    // Ignoramos las repeticiones automaticas del teclado
    if (evento->isAutoRepeat())
    {
        return;
    }

    // Dejamos de solicitar movimiento hacia la izquierda
    if (evento->key() == Qt::Key_A)
    {
        izquierdaPresionada = false;
    }

    // Dejamos de solicitar movimiento hacia la derecha
    else if (evento->key() == Qt::Key_D)
    {
        derechaPresionada = false;
    }

    else if (evento->key() == Qt::Key_W)
    {
        saltarPresionado = false;
    }

    else
    {
        QGraphicsView::keyReleaseEvent(evento);
    }
}

//Actualiza la posicion de dexter
void VistaNivel1::actualizarJuego()
{
    if (partidaTerminada)
    {
        return;
    }

    int direccion = 0;

    if (izquierdaPresionada)
    {
        direccion--;
    }

    if (derechaPresionada)
    {
        direccion++;
    }
    if (saltarPresionado)
    {
        nivel.saltar();
    }

    nivel.actualizar(direccion, 0.016);

    figuraDexter->setPos(nivel.getDexter().getX(), nivel.getDexter().getY());
    actualizarAnimacion(direccion);
    actualizarEfectoDano();//Agrega el efecto de parpadeo cuando recibe daño Dexter

    // Actualizamos las muestras que aparecen en pantalla
    actualizarMuestrasGraficas();

    // Actualizamos las semillas que aparecen en pantalla
    actualizarSemillasGraficas();

    // Actualizamos los beneficios que aparecen en pantalla
    actualizarBeneficiosGraficos();

    // Actualizamos el contador de muestras
    actualizarContadorMuestras();

    // Actualizamos el tiempo restante
    QString nuevoTiempo = "Tiempo: "
                          + QString::number(nivel.getTiempoRestante())
                          + " s";

    // Solo modificamos el texto cuando cambia
    if (textoTiempo->toPlainText() != nuevoTiempo)
    {
        textoTiempo->setPlainText(nuevoTiempo);
    }

    // Actualizamos el contador de vidas
    actualizarContadorVidas();

    //Actualizamos las homigas que aparecen en la pantalla
    actualizarHormigasGraficas();
    \
    //Actualizamos los jabalis y advertencias que aparecen en la pantalla
    actualizarJabalisGraficos();
    actualizarAdvertenciaJabali();

    //Actualizamos la Guacamaya que aparece en la pantalla
    actualizarGuacamayasGraficas();

    //Actualizamos el escremento que aparece en la pantalla
    actualizarExcrementosGraficos();

    // Comprobamos si Dexter ha perdido
    if (nivel.haGanado() || nivel.haPerdido())
    {
        finalizarPartida();
    }
}

// Actualizamos la animacion y orientacion de Dexter
void VistaNivel1::actualizarAnimacion(int direccion)
{
    // Guardamos la orientacion anterior
    bool orientacionAnterior = mirandoDerecha;

    // Determinamos hacia donde mira Dexter
    if (direccion > 0)
    {
        mirandoDerecha = true;
    }
    else if (direccion < 0)
    {
        mirandoDerecha = false;
    }

    // Variables para controlar la animacion
    int nuevoEstado;
    int duracionFotograma;
    int cantidadFotogramas;

    // Puntero al vector de la animacion actual
    // Evitamos copiar todos los fotogramas
    vector<QPixmap> *spritesActuales;

    // Estado 3: Dexter esta utilizando la red
    // La recoleccion tiene prioridad sobre otras animaciones
    if (nivel.getDexter().estaRecolectando())
    {
        nuevoEstado = 3;
        duracionFotograma = 80;
        spritesActuales = &spritesRecoleccion;
    }

    // Estado 2: Dexter se encuentra en el aire
    else if (nivel.getDexter().getY() +
                 nivel.getDexter().getAlto() < 650)
    {
        nuevoEstado = 2;
        duracionFotograma = 100;
        spritesActuales = &spritesSalto;
    }

    // Estado 1: Dexter se encuentra caminando
    else if (direccion != 0)
    {
        nuevoEstado = 1;
        duracionFotograma = 100;
        spritesActuales = &spritesCaminata;
    }

    // Estado 0: Dexter permanece quieto
    else
    {
        nuevoEstado = 0;
        duracionFotograma = 200;
        spritesActuales = &spritesReposo;
    }

    // Obtenemos la cantidad de fotogramas
    // almacenados en el vector seleccionado
    cantidadFotogramas = spritesActuales->size();

    // Indicamos si es necesario cambiar la imagen
    bool cambiarFotograma = false;

    // Reiniciamos la animacion si cambia el estado
    if (nuevoEstado != estadoAnimacion)
    {
        estadoAnimacion = nuevoEstado;
        fotogramaActual = 0;
        tiempoAnimacion = 0;

        cambiarFotograma = true;
    }

    // Continuamos la animacion actual
    else if (cantidadFotogramas > 1)
    {
        // Acumulamos el tiempo transcurrido
        tiempoAnimacion += 16;

        // Comprobamos si debemos avanzar
        // al siguiente fotograma
        if (tiempoAnimacion >= duracionFotograma)
        {
            tiempoAnimacion -= duracionFotograma;
            fotogramaActual++;

            // Reiniciamos al finalizar la animacion
            if (fotogramaActual >= cantidadFotogramas)
            {
                fotogramaActual = 0;
            }

            cambiarFotograma = true;
        }
    }

    // Cambiamos la imagen solamente cuando cambia
    // el fotograma o la orientacion de Dexter
    if (cambiarFotograma ||
        orientacionAnterior != mirandoDerecha)
    {
        mostrarFotograma(
            (*spritesActuales)[fotogramaActual]
            );
    }
}


// Cargamos los fotogramas de una animacion
// dentro de su respectivo vector
void VistaNivel1::cargarAnimacion(
    vector<QPixmap> &sprites,
    QString ruta, int cantidad)
{
    // Accedemos a la carpeta de recursos
    QDir directorio(ruta);

    // Obtenemos los nombres de los archivos PNG
    // ordenados alfabeticamente
    QStringList archivos = directorio.entryList(
        QStringList() << "*.png",
        QDir::Files,
        QDir::Name
        );

    // Comprobamos que existan suficientes imagenes
    if (archivos.size() < cantidad)
    {
        throw runtime_error(
            ("Faltan sprites en: " + ruta).toStdString()
            );
    }

    // Reservamos la capacidad necesaria del vector
    // para evitar reasignaciones innecesarias
    sprites.reserve(cantidad);

    // Recorremos los archivos de la animacion
    for (int i = 0; i < cantidad; i++)
    {
        // Construimos la ruta completa del archivo
        QString nombre = ruta + "/" + archivos[i];

        // Cargamos la imagen
        QPixmap imagen(nombre);

        // Comprobamos que se cargo correctamente
        if (imagen.isNull())
        {
            throw runtime_error(
                ("No se pudo cargar: " + nombre).toStdString()
                );
        }

        // Almacenamos el fotograma en el vector
        sprites.push_back(imagen);
    }
}


// Cargamos todas las animaciones de Dexter
void VistaNivel1::cargarSprites()
{
    // Reposo: utilizamos una sola imagen
    // para que Dexter permanezca quieto
    cargarAnimacion(spritesReposo,
                    ":/sprites/dexter/reposo", 1);

    // Caminata: seis fotogramas
    cargarAnimacion(spritesCaminata,
                    ":/sprites/dexter/caminata", 6);

    // Salto: cuatro fotogramas
    cargarAnimacion(spritesSalto,
                    ":/sprites/dexter/salto", 4);

    // Recoleccion: cinco fotogramas
    cargarAnimacion(spritesRecoleccion,
                    ":/sprites/dexter/recoleccion", 5);
}


// Mostramos la imagen segun la orientacion
// actual del personaje
void VistaNivel1::mostrarFotograma(
    const QPixmap &imagen)
{
    // Si Dexter mira hacia la derecha,
    // utilizamos la imagen original
    if (mirandoDerecha)
    {
        figuraDexter->setPixmap(imagen);

        // Alineamos la imagen con el cuerpo fisico
        figuraDexter->setOffset(-19.5, -25);
    }

    // Si Dexter mira hacia la izquierda,
    // invertimos horizontalmente la imagen
    else
    {
        QPixmap imagenInvertida = imagen.transformed(
            QTransform().scale(-1, 1)
            );

        // Mostramos la imagen invertida
        figuraDexter->setPixmap(imagenInvertida);

        // Compensamos el desplazamiento de la imagen
        // para conservar el centro del cuerpo fisico
        figuraDexter->setOffset(
            44.5 - imagen.width(), -25
            );
    }
}

// Sincronizamos las muestras graficas de los recolectables con la logica
void VistaNivel1::actualizarMuestrasGraficas()
{
    // Consultamos las muestras sin copiar el vector
    const vector<Recolectables*> &muestras =
        nivel.getMuestras();

    // Creamos figuras cuando aparecen nuevas muestras
    while (figurasMuestras.size() < muestras.size())
    {
        QGraphicsPixmapItem *figura =
            escena->addPixmap(QPixmap());

        figurasMuestras.push_back(figura);
    }

    // Eliminamos las figuras que ya no necesitamos
    while (figurasMuestras.size() > muestras.size())
    {
        delete figurasMuestras.back();

        figurasMuestras.pop_back();
    }

    // Actualizamos las muestras graficas
    for (int i = 0; i < muestras.size(); i++)
    {
        // Actualizamos la posicion de la figura
        figurasMuestras[i]->setPos(
            muestras[i]->getX(),
            muestras[i]->getY()
            );

        // Puntero al sprite correspondiente
        const QPixmap *imagen;

        // Seleccionamos la imagen especial
        if (muestras[i]->esEspecial())
        {
            imagen = &spriteMuestraEspecial;
        }

        // Seleccionamos la variante de flor comun
        else
        {
            imagen = &spritesMuestrasComunes[
                muestras[i]->getVariante()
            ];
        }

        // Cambiamos la imagen solamente si es necesario
        if (figurasMuestras[i]->pixmap().cacheKey()
            != imagen->cacheKey())
        {
            figurasMuestras[i]->setPixmap(*imagen);
        }
    }
}

// Cargamos las imagenes de las muestras botanicas
void VistaNivel1::cargarSpritesMuestras()
{
    // Cargamos las cuatro variantes de flores comunes
    cargarAnimacion(spritesMuestrasComunes,
                    ":/sprites/objetos/comunes", 4);

    // Cargamos la flor especial
    spriteMuestraEspecial.load(
        ":/sprites/objetos/especial/especial_00.png");

    // Comprobamos que la imagen se cargo correctamente
    if (spriteMuestraEspecial.isNull())
    {
        throw runtime_error(
            "No se pudo cargar la muestra especial");
    }
    // Ajustamos las dimensiones de las flores comunes
    for (int i = 0; i < spritesMuestrasComunes.size(); i++)
    {
        spritesMuestrasComunes[i] =
            spritesMuestrasComunes[i].scaled(
                40, 40, Qt::IgnoreAspectRatio,
                Qt::FastTransformation);
    }

    // Ajustamos las dimensiones de la flor especial
    spriteMuestraEspecial = spriteMuestraEspecial.scaled(
        40, 40, Qt::IgnoreAspectRatio,
        Qt::FastTransformation);
}

// Creamos el contador de muestras botanicas
void VistaNivel1::crearContadorMuestras()
{
    // Inicializamos la ultima cantidad mostrada
    muestrasMostradas = -1;

    // Creamos el fondo del contador
    escena->addRect(
              20, 20, 245, 54,
              QPen(QColor(140, 170, 112), 2),
              QBrush(QColor(38, 59, 52))
              )->setZValue(20);

    // Creamos el texto del contador
    textoMuestras = escena->addText("");

    // Configuramos la fuente y el color
    textoMuestras->setFont(
        QFont("Consolas", 16, QFont::Bold));

    textoMuestras->setDefaultTextColor(
        QColor(255, 245, 217));

    // Ubicamos el texto sobre el fondo
    textoMuestras->setPos(30, 30);
    textoMuestras->setZValue(21);

    // Mostramos la cantidad inicial
    actualizarContadorMuestras();
}

// Actualizamos la cantidad de muestras recolectadas
void VistaNivel1::actualizarContadorMuestras()
{
    int cantidad = nivel.getMuestrasRecolectadas();

    // Evitamos modificar el texto si nada ha cambiado
    if (cantidad == muestrasMostradas)
    {
        return;
    }

    muestrasMostradas = cantidad;

    // Mostramos las muestras recolectadas y el objetivo
    textoMuestras->setPlainText(
        "Muestras: " + QString::number(cantidad) + " / 20"
        );
}

// Cargamos la imagen de la semilla una sola vez
void VistaNivel1::cargarSpriteSemilla()
{
    spriteSemilla.load(
        ":/sprites/objetos/semilla/semilla_00.png");

    if (spriteSemilla.isNull())
    {
        throw runtime_error(
            "No se pudo cargar la semilla explosiva");
    }

    // Ajustamos la imagen a sus dimensiones fisicas
    spriteSemilla = spriteSemilla.scaled(
        40, 40,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation);
}

void VistaNivel1::actualizarSemillasGraficas()
{
    const vector<SemillaExplosiva*>& semillas =
        nivel.getSemillas();

    // Creamos las figuras necesarias
    while (figurasSemillas.size() < semillas.size())
    {
        QGraphicsPixmapItem *figura =
            escena->addPixmap(spriteSemilla);

        figurasSemillas.push_back(figura);
    }

    // Eliminamos las figuras sobrantes
    while (figurasSemillas.size() > semillas.size())
    {
        delete figurasSemillas.back();

        figurasSemillas.pop_back();
    }

    // Actualizamos las figuras existentes
    for (int i = 0; i < semillas.size(); i++)
    {
        const QPixmap *imagen = &spriteSemilla;

        float posicionX = semillas[i]->getX();
        float posicionY = semillas[i]->getY();

        // Si exploto, seleccionamos su fotograma
        if (semillas[i]->haExplotado())
        {
            int fotograma =
                semillas[i]->getFotogramaExplosion();

            imagen = &spritesExplosion[fotograma];

            // Centramos la explosion sobre la semilla
            posicionX -= 40;
            posicionY -= 40;
        }

        // Solo cambiamos la imagen si es diferente
        if (figurasSemillas[i]->pixmap().cacheKey()
            != imagen->cacheKey())
        {
            figurasSemillas[i]->setPixmap(*imagen);
        }

        // Actualizamos la posicion
        figurasSemillas[i]->setPos(
            posicionX, posicionY);
    }
}

void VistaNivel1::cargarSpritesExplosion()
{
    // Cargamos los ocho fotogramas
    cargarAnimacion(
        spritesExplosion,
        ":/sprites/objetos/explosion", 8);

    // Ajustamos el tamaño de cada fotograma
    for (int i = 0; i < spritesExplosion.size(); i++)
    {
        spritesExplosion[i] =
            spritesExplosion[i].scaled(
                120, 120,
                Qt::IgnoreAspectRatio,
                Qt::FastTransformation);
    }
}

void VistaNivel1::actualizarEfectoDano()
{
    bool activar = nivel.getDexter().debeTitilarRojo();

    // Solo modificamos el efecto si cambia su estado
    if (efectoDano->isEnabled() != activar)
    {
        efectoDano->setEnabled(activar);
    }
}

void VistaNivel1::crearContadorVidas()
{
    vidasMostradas = -1;

    // Fondo del contador
    escena->addRect(
              20, 84, 245, 54,
              QPen(QColor(140, 170, 112), 2),
              QBrush(QColor(38, 59, 52))
              )->setZValue(20);

    // Texto del contador
    textoVidas = escena->addText("");

    textoVidas->setFont(
        QFont("Consolas", 16, QFont::Bold));

    textoVidas->setDefaultTextColor(
        QColor(255, 245, 217));

    textoVidas->setPos(30, 94);
    textoVidas->setZValue(21);

    actualizarContadorVidas();
}

void VistaNivel1::actualizarContadorVidas()
{
    int cantidad = nivel.getDexter().getVidas();

    // Solo actualizamos cuando cambian las vidas
    if (cantidad == vidasMostradas)
    {
        return;
    }

    vidasMostradas = cantidad;

    textoVidas->setPlainText(
        "Vidas: " + QString::number(cantidad) + " / 3"
        );
}

void VistaNivel1::cargarSpriteMuerte()
{
    spriteMuerte.load(
        ":/sprites/dexter/muerte/dexter_muerte_00.png");

    if (spriteMuerte.isNull())
    {
        throw runtime_error(
            "No se pudo cargar el sprite de muerte");
    }
}

void VistaNivel1::finalizarPartida()
{
    // Evitamos finalizar la partida varias veces
    if (partidaTerminada)
    {
        return;
    }

    partidaTerminada = true;

    figuraAdvertencia->hide();

    // Detenemos el temporizador principal
    temporizador->stop();

    // Desactivamos el parpadeo rojo
    efectoDano->setEnabled(false);

    // Mostramos el sprite de muerte solamente
    // cuando Dexter pierde todas sus vidas
    if (nivel.getDexter().getVidas() <= 0)
    {
        figuraDexter->setPixmap(spriteMuerte);

        // Eliminamos el desplazamiento anterior
        figuraDexter->setOffset(0, 0);

        // Calculamos las dimensiones graficas
        float anchoSprite =
            spriteMuerte.width() * figuraDexter->scale();

        float altoSprite =
            spriteMuerte.height() * figuraDexter->scale();

        // Centramos el sprite respecto al cuerpo de Dexter
        float posicionX =
            nivel.getDexter().getX() +
            nivel.getDexter().getAncho() / 2.0 -
            anchoSprite / 2;

        // Evitamos que quede fuera del escenario
        if (posicionX < 0)
        {
            posicionX = 0;
        }

        if (posicionX + anchoSprite > 1280)
        {
            posicionX = 1280 - anchoSprite;
        }

        // Colocamos a Dexter acostado sobre el suelo
        figuraDexter->setPos(
            posicionX, 650 - altoSprite);
    }

    // Fondo del mensaje de finalizacion
    escena->addRect(
              340, 210, 600, 280,
              QPen(QColor(140, 170, 112), 3),
              QBrush(QColor(38, 59, 52))
              )->setZValue(30);

    // Determinamos el resultado de la partida
    QString resultado;

    if (nivel.haGanado())
    {
        resultado = "¡VICTORIA!";
    }
    else if (nivel.getDexter().getVidas() <= 0)
    {
        resultado = "¡HAS PERDIDO!";
    }
    else
    {
        resultado = "¡TIEMPO AGOTADO!";
    }

    // Creamos el mensaje de finalizacion
    QGraphicsTextItem *mensaje =
        escena->addText(resultado);

    mensaje->setFont(
        QFont("Consolas", 28, QFont::Bold));

    mensaje->setDefaultTextColor(
        QColor(255, 245, 217));

    // Centramos el mensaje
    mensaje->setPos(
        (1280 - mensaje->boundingRect().width()) / 2,
        260
        );

    mensaje->setZValue(31);

    // Creamos el boton para regresar al menu principal
    QPushButton *regresar =
        new QPushButton("VOLVER AL MENU PRINCIPAL");

    regresar->setFixedSize(360, 50);

    regresar->setFont(
        QFont("Consolas", 14, QFont::Bold));

    // Incorporamos el boton a la escena
    QGraphicsProxyWidget *botonRegresar =
        escena->addWidget(regresar);

    botonRegresar->setPos(460, 390);
    botonRegresar->setZValue(31);

    // Reutilizamos la señal de navegacion
    connect(regresar, &QPushButton::clicked,
            this, &VistaNivel1::solicitarMenuPrincipal);
}

void VistaNivel1::cargarSpritesBeneficios()
{
    // Cargamos las imagenes
    spriteMedicinal.load(
        ":/sprites/objetos/medicinal/medicinal_00.png");

    spriteSuero.load(
        ":/sprites/objetos/suero/suero_00.png");

    // Verificamos que se hayan cargado correctamente
    if (spriteMedicinal.isNull())
    {
        throw runtime_error(
            "No se pudo cargar la planta medicinal");
    }

    if (spriteSuero.isNull())
    {
        throw runtime_error(
            "No se pudo cargar el suero");
    }

    // Ajustamos las dimensiones de las imagenes
    spriteMedicinal = spriteMedicinal.scaled(
        40, 40,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation);

    spriteSuero = spriteSuero.scaled(
        40, 40,
        Qt::IgnoreAspectRatio,
        Qt::FastTransformation);
}

void VistaNivel1::actualizarBeneficiosGraficos()
{
    // Consultamos los beneficios existentes
    const vector<Beneficios*>& beneficios =
        nivel.getBeneficios();

    // Creamos las figuras que hagan falta
    while (figurasBeneficios.size() < beneficios.size())
    {
        QGraphicsPixmapItem *figura =
            escena->addPixmap(QPixmap());

        figurasBeneficios.push_back(figura);
    }

    // Eliminamos las figuras sobrantes
    while (figurasBeneficios.size() > beneficios.size())
    {
        delete figurasBeneficios.back();

        figurasBeneficios.pop_back();
    }

    // Actualizamos las figuras existentes
    for (int i = 0; i < beneficios.size(); i++)
    {
        const QPixmap *imagen;

        // Seleccionamos el sprite correspondiente
        if (beneficios[i]->getTipo() == 0)
        {
            imagen = &spriteMedicinal;
        }
        else
        {
            imagen = &spriteSuero;
        }

        // Cambiamos la imagen solo cuando sea necesario
        if (figurasBeneficios[i]->pixmap().cacheKey()
            != imagen->cacheKey())
        {
            figurasBeneficios[i]->setPixmap(*imagen);
        }

        // Actualizamos la posicion grafica
        figurasBeneficios[i]->setPos(
            beneficios[i]->getX(),
            beneficios[i]->getY());
    }
}

//--------------------------------------------------- Hormigas ----------------------------------//

void VistaNivel1::cargarSpritesHormiga()
{
    cargarAnimacion(
        spritesHormiga,
        ":/sprites/enemigos/hormiga", 8);

    for (int i = 0; i < spritesHormiga.size(); i++)
    {
        spritesHormiga[i] =
            spritesHormiga[i].scaled(
                70, 50,
                Qt::IgnoreAspectRatio,
                Qt::FastTransformation);
    }
}


void VistaNivel1::actualizarHormigasGraficas()
{
    const vector<Hormiga*>& hormigas =
        nivel.getHormigas();

    // Creamos las figuras necesarias
    while (figurasHormigas.size() < hormigas.size())
    {
        QGraphicsPixmapItem *figura =
            escena->addPixmap(spritesHormiga[0]);

        figurasHormigas.push_back(figura);
    }

    // Eliminamos las figuras sobrantes
    while (figurasHormigas.size() > hormigas.size())
    {
        delete figurasHormigas.back();

        figurasHormigas.pop_back();
    }

    // Si no hay hormigas, no actualizamos la animacion
    if (hormigas.empty())
    {
        return;
    }

    // Avanzamos la animacion
    tiempoAnimacionHormiga += 16;

    if (tiempoAnimacionHormiga >= 100)
    {
        tiempoAnimacionHormiga -= 100;

        fotogramaHormiga++;

        if (fotogramaHormiga >= 8)
        {
            fotogramaHormiga = 0;
        }
    }

    const QPixmap &imagen =
        spritesHormiga[fotogramaHormiga];

    for (int i = 0; i < hormigas.size(); i++)
    {
        // Cambiamos la imagen solo cuando sea necesario
        if (figurasHormigas[i]->pixmap().cacheKey()
            != imagen.cacheKey())
        {
            figurasHormigas[i]->setPixmap(imagen);
        }

        // Orientamos la figura sin duplicar los sprites
        if (hormigas[i]->getDireccion() == 1)
        {
            figurasHormigas[i]->setTransform(
                QTransform());
        }
        else
        {
            QTransform transformacion;

            transformacion.translate(imagen.width(), 0);
            transformacion.scale(-1, 1);

            figurasHormigas[i]->setTransform(
                transformacion);
        }

        // Actualizamos la posicion
        figurasHormigas[i]->setPos(
            hormigas[i]->getX(),
            hormigas[i]->getY());
    }
}


void VistaNivel1::cargarSpritesJabali()
{
    // Cargamos los ocho fotogramas del jabali
    cargarAnimacion(
        spritesJabali,
        ":/sprites/enemigos/jabali", 8);

    // Cargamos los siete fotogramas de advertencia
    cargarAnimacion(
        spritesAdvertencia,
        ":/sprites/enemigos/advertencia", 7);

    // Ajustamos el tamano del jabali
    for (int i = 0; i < spritesJabali.size(); i++)
    {
        spritesJabali[i] =
            spritesJabali[i].scaled(
                110, 60,
                Qt::IgnoreAspectRatio,
                Qt::FastTransformation);
    }

    // Ajustamos el tamano de la advertencia
    for (int i = 0; i < spritesAdvertencia.size(); i++)
    {
        spritesAdvertencia[i] =
            spritesAdvertencia[i].scaled(
                60, 80,
                Qt::KeepAspectRatio,
                Qt::FastTransformation);
    }
}

void VistaNivel1::actualizarJabalisGraficos()
{
    const vector<Jabali*>& jabalis =
        nivel.getJabalis();

    // Creamos las figuras necesarias
    while (figurasJabalis.size() < jabalis.size())
    {
        QGraphicsPixmapItem *figura =
            escena->addPixmap(spritesJabali[0]);

        figurasJabalis.push_back(figura);
    }

    // Eliminamos las figuras sobrantes
    while (figurasJabalis.size() > jabalis.size())
    {
        delete figurasJabalis.back();

        figurasJabalis.pop_back();
    }

    // No procesamos animaciones si no hay jabalis
    if (jabalis.empty())
    {
        return;
    }

    // Actualizamos el fotograma
    tiempoAnimacionJabali += 16;

    if (tiempoAnimacionJabali >= 60)
    {
        tiempoAnimacionJabali -= 60;

        fotogramaJabali++;

        if (fotogramaJabali >= 8)
        {
            fotogramaJabali = 0;
        }
    }

    const QPixmap &imagen =
        spritesJabali[fotogramaJabali];

    for (int i = 0; i < jabalis.size(); i++)
    {
        // Cambiamos la imagen cuando sea necesario
        if (figurasJabalis[i]->pixmap().cacheKey()
            != imagen.cacheKey())
        {
            figurasJabalis[i]->setPixmap(imagen);
        }

        // Calculamos la orientacion
        QTransform transformacion;

        if (jabalis[i]->getDireccion() == -1)
        {
            transformacion.translate(imagen.width(), 0);

            transformacion.scale(-1, 1);
        }

        // Solo modificamos la transformacion si cambia
        if (figurasJabalis[i]->transform()
            != transformacion)
        {
            figurasJabalis[i]->setTransform(
                transformacion);
        }

        // Actualizamos la posicion
        figurasJabalis[i]->setPos(
            jabalis[i]->getX(),
            jabalis[i]->getY());
    }
}



void VistaNivel1::actualizarAdvertenciaJabali()
{
    // Si no existe una advertencia, ocultamos la figura
    if (!nivel.hayAdvertenciaJabali())
    {
        figuraAdvertencia->hide();

        fotogramaAdvertencia = 0;
        tiempoAnimacionAdvertencia = 0;

        return;
    }

    // Actualizamos la animacion
    tiempoAnimacionAdvertencia += 16;

    if (tiempoAnimacionAdvertencia >= 100)
    {
        tiempoAnimacionAdvertencia -= 100;

        fotogramaAdvertencia++;

        if (fotogramaAdvertencia >= 7)
        {
            fotogramaAdvertencia = 0;
        }
    }

    const QPixmap &imagen =
        spritesAdvertencia[fotogramaAdvertencia];

    // Actualizamos el sprite si ha cambiado
    if (figuraAdvertencia->pixmap().cacheKey()
        != imagen.cacheKey())
    {
        figuraAdvertencia->setPixmap(imagen);
    }

    // Determinamos en que extremo mostrar la advertencia
    if (nivel.getDireccionJabali() == 1)
    {
        figuraAdvertencia->setPos(
            20, 550 - imagen.height());
    }
    else
    {
        figuraAdvertencia->setPos(
            1260 - imagen.width(),
            550 - imagen.height());
    }

    // Hacemos visible la advertencia
    figuraAdvertencia->show();
}


void VistaNivel1::cargarSpritesGuacamaya()
{
    cargarAnimacion(
        spritesGuacamaya,
        ":/sprites/enemigos/guacamaya", 7);

    for (int i = 0; i < spritesGuacamaya.size(); i++)
    {
        spritesGuacamaya[i] =
            spritesGuacamaya[i].scaled(
                100, 70,
                Qt::IgnoreAspectRatio,
                Qt::FastTransformation);
    }
}

void VistaNivel1::actualizarGuacamayasGraficas()
{
    const vector<Guacamaya*>& guacamayas =
        nivel.getGuacamayas();

    // Creamos las figuras necesarias
    while (figurasGuacamayas.size() < guacamayas.size())
    {
        QGraphicsPixmapItem *figura =
            escena->addPixmap(spritesGuacamaya[0]);

        figurasGuacamayas.push_back(figura);
    }

    // Eliminamos las figuras sobrantes
    while (figurasGuacamayas.size() > guacamayas.size())
    {
        delete figurasGuacamayas.back();

        figurasGuacamayas.pop_back();
    }

    // Evitamos procesar animaciones innecesarias
    if (guacamayas.empty())
    {
        return;
    }

    // Actualizamos el fotograma de vuelo
    tiempoAnimacionGuacamaya += 16;

    if (tiempoAnimacionGuacamaya >= 90)
    {
        tiempoAnimacionGuacamaya -= 90;

        fotogramaGuacamaya++;

        if (fotogramaGuacamaya >= spritesGuacamaya.size())
        {
            fotogramaGuacamaya = 0;
        }
    }

    const QPixmap &imagen =
        spritesGuacamaya[fotogramaGuacamaya];

    for (int i = 0; i < guacamayas.size(); i++)
    {
        // Actualizamos la imagen solo si cambia
        if (figurasGuacamayas[i]->pixmap().cacheKey()
            != imagen.cacheKey())
        {
            figurasGuacamayas[i]->setPixmap(imagen);
        }

        // Determinamos la orientacion
        QTransform transformacion;

        if (guacamayas[i]->getDireccion() == -1)
        {
            transformacion.translate(imagen.width(), 0);

            transformacion.scale(-1, 1);
        }

        // Evitamos transformaciones innecesarias
        if (figurasGuacamayas[i]->transform()
            != transformacion)
        {
            figurasGuacamayas[i]->setTransform(
                transformacion);
        }

        // Actualizamos la posicion
        figurasGuacamayas[i]->setPos(
            guacamayas[i]->getX(),
            guacamayas[i]->getY());
    }
}


void VistaNivel1::cargarSpritesExcrementos()
{
    spriteExcrementoCaida.load(
        ":/sprites/objetos/excremento/excremento_caida_00.png");

    spriteExcrementoSuelo.load(
        ":/sprites/objetos/excremento/excremento_suelo_00.png");

    if (spriteExcrementoCaida.isNull() ||
        spriteExcrementoSuelo.isNull())
    {
        throw runtime_error(
            "No se pudieron cargar los excrementos");
    }

    spriteExcrementoCaida =
        spriteExcrementoCaida.scaled(
            25, 30,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation);

    spriteExcrementoSuelo =
        spriteExcrementoSuelo.scaled(
            45, 15,
            Qt::IgnoreAspectRatio,
            Qt::FastTransformation);
}

void VistaNivel1::actualizarExcrementosGraficos()
{
    const vector<Excremento*>& excrementos =
        nivel.getExcrementos();

    // Creamos las figuras necesarias
    while (figurasExcrementos.size() < excrementos.size())
    {
        QGraphicsPixmapItem *figura =
            escena->addPixmap(spriteExcrementoCaida);

        figurasExcrementos.push_back(figura);
    }

    // Eliminamos las figuras sobrantes
    while (figurasExcrementos.size() > excrementos.size())
    {
        delete figurasExcrementos.back();

        figurasExcrementos.pop_back();
    }

    for (int i = 0; i < excrementos.size(); i++)
    {
        const QPixmap *imagen = &spriteExcrementoCaida;

        // Seleccionamos el sprite correspondiente
        if (excrementos[i]->estaEnSuelo())
        {
            imagen = &spriteExcrementoSuelo;
        }

        // Cambiamos la imagen solo cuando sea necesario
        if (figurasExcrementos[i]->pixmap().cacheKey()
            != imagen->cacheKey())
        {
            figurasExcrementos[i]->setPixmap(*imagen);
        }

        // Actualizamos la posicion
        figurasExcrementos[i]->setPos(
            excrementos[i]->getX(),
            excrementos[i]->getY());
    }
}

void VistaNivel1::alternarPausa()
{
    // No podemos pausar una partida finalizada
    if (partidaTerminada)
    {
        return;
    }
    partidaPausada = !partidaPausada;

    // Reiniciamos el estado de las teclas
    izquierdaPresionada = false;
    derechaPresionada = false;
    saltarPresionado = false;

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

        // Recuperamos el control del teclado
        setFocus();

        temporizador->start();
    }
}