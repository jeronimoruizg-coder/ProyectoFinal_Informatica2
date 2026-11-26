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

VistaNivel1::VistaNivel1() {
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
    }
    catch (const runtime_error &error)
    {
        // Informamos que recurso produjo el error
        QMessageBox::critical(this, "Error de recursos",
                              error.what());

        // Detenemos la inicializacion del nivel
        return;
    }

    // Mostramos el primer fotograma de reposo
    figuraDexter = escena->addPixmap(spritesReposo[0]);
    figuraDexter->setScale(2);  // Duplicamos el tamaño visual de la imagen
    figuraDexter->setOffset(-19.5, -25); // Ajustamos la imagen respecto a la posicion fisica
    figuraDexter->setPos(nivel.getDexter().getX(), nivel.getDexter().getY());//Le damos una posicion inicial a Dexter

    // Creamos el panel de muestras botanicas
    crearContadorMuestras();

    //Detalles de la vista grafica
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//Desactiva el desplazamiento Horizontal
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);// Desactiva el desplazamiento Vertical
    setFrameShape(QFrame::NoFrame);// Elimina el borde que Qt coloca alrededor de la vista.
    setFixedSize(1280, 720);// Le damos el tamaño fijo a nuestra Vista grafica, lo que se va a ver al final

    // Creamos el temporizador
    temporizador = new QTimer(this);
    // Conectamos la señal con nuestra funcion
    connect(temporizador, &QTimer::timeout,this, &VistaNivel1::actualizarJuego);
    temporizador->start(16);

}

// Detectamos cuando el jugador presiona una tecla
void VistaNivel1::keyPressEvent(QKeyEvent *evento)
{
    // Ignoramos las repeticiones automaticas del teclado
    if (evento->isAutoRepeat())
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

    // Actualizamos las muestras que aparecen en pantalla
    actualizarMuestrasGraficas();

    // Actualizamos el contador de muestras
    actualizarContadorMuestras();
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

    // Daño: cuatro fotogramas
    cargarAnimacion(spritesDano,
                    ":/sprites/dexter/dano", 4);
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