#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "vistanivel1.h"
#include "vistanivel2.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QIcon>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mostrarMenuPrincipal();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mostrarMenuPrincipal()
{
    QWidget *menu = new QWidget(this);
    menu->setFixedSize(1280, 720);

    // Opciones iniciales
    nivelSeleccionado = 1;
    dificultadSeleccionada = false;

    // Fondo del menu
    QLabel *fondo = new QLabel(menu);

    fondo->setPixmap(
        QPixmap(":/sprites/menu/fondo_menu.png"));

    fondo->setGeometry(0, 0, 1280, 720);

    // Panel de nivel
    QLabel *panelNivel = new QLabel(menu);

    panelNivel->setPixmap(
        QPixmap(":/sprites/menu/selector_nivel.png"));

    panelNivel->setGeometry(479, 274, 322, 86);

    // Panel de dificultad
    QLabel *panelDificultad = new QLabel(menu);

    panelDificultad->setPixmap(
        QPixmap(":/sprites/menu/selector_dificultad.png"));

    panelDificultad->setGeometry(478, 367, 323, 86);

    // Texto del nivel
    textoNivel = new QLabel("Nivel 1", menu);

    textoNivel->setGeometry(550, 311, 180, 36);
    textoNivel->setAlignment(Qt::AlignCenter);
    textoNivel->setFont(QFont("Arial", 19, QFont::Bold));
    textoNivel->setStyleSheet("color: white;");

    // Texto de la dificultad
    textoDificultad = new QLabel("Normal", menu);

    textoDificultad->setGeometry(550, 404, 180, 36);
    textoDificultad->setAlignment(Qt::AlignCenter);
    textoDificultad->setFont(QFont("Arial", 19, QFont::Bold));
    textoDificultad->setStyleSheet("color: white;");

    // Flechas para seleccionar el nivel
    QPushButton *nivelIzquierda = crearBotonGrafico(
        menu, ":/sprites/menu/flecha_izquierda.png",
        499, 303);

    QPushButton *nivelDerecha = crearBotonGrafico(
        menu, ":/sprites/menu/flecha_derecha.png",
        751, 303);

    // Flechas para seleccionar la dificultad
    QPushButton *dificultadIzquierda = crearBotonGrafico(
        menu, ":/sprites/menu/flecha_izquierda.png",
        499, 396);

    QPushButton *dificultadDerecha = crearBotonGrafico(
        menu, ":/sprites/menu/flecha_derecha.png",
        751, 396);

    // Botones principales
    QPushButton *botonJugar = crearBotonGrafico(
        menu, ":/sprites/menu/boton_jugar.png",
        455, 459);

    QPushButton *botonSalir = crearBotonGrafico(
        menu, ":/sprites/menu/boton_salir.png",
        470, 563);

    // Conexiones de las flechas
    connect(nivelIzquierda, &QPushButton::clicked,
            this, &MainWindow::cambiarNivel);

    connect(nivelDerecha, &QPushButton::clicked,
            this, &MainWindow::cambiarNivel);

    connect(dificultadIzquierda, &QPushButton::clicked,
            this, &MainWindow::cambiarDificultad);

    connect(dificultadDerecha, &QPushButton::clicked,
            this, &MainWindow::cambiarDificultad);

    // Conexiones de los botones principales
    connect(botonJugar, &QPushButton::clicked,
            this, &MainWindow::jugar);

    connect(botonSalir, &QPushButton::clicked,
            this, &MainWindow::close);

    setCentralWidget(menu);
}

void MainWindow::iniciarNivel1(bool dificultad)
{
    dificultadSeleccionada = dificultad;//Garantizamos que la dificultad seleccionada actue en ambos niveles

    VistaNivel1 *vistaNivel1 =
        new VistaNivel1(dificultad);

    // Conectamos la solicitud de regresar al menu
    connect(vistaNivel1,
            &VistaNivel1::solicitarMenuPrincipal,
            this,
            &MainWindow::mostrarMenuPrincipal,
            Qt::QueuedConnection);


    // Conectamos la solicitud de avanzar al nivel 2
    connect(vistaNivel1,
            &VistaNivel1::solicitarSiguienteNivel,
            this,
            &MainWindow::avanzarAlNivel2,
            Qt::QueuedConnection);


    // Mostramos el nivel
    setCentralWidget(vistaNivel1);

    vistaNivel1->setFocus();
}

void MainWindow::iniciarNivel2(bool dificultad)
{
    // Creamos el nivel 2 con la dificultad seleccionada
    VistaNivel2 *vistaNivel2 =
        new VistaNivel2(dificultad);

    // Conectamos la solicitud de regresar al menu
    connect(vistaNivel2,
            &VistaNivel2::solicitarMenuPrincipal,
            this,
            &MainWindow::mostrarMenuPrincipal,
            Qt::QueuedConnection);

    // Mostramos el nivel
    setCentralWidget(vistaNivel2);

    // Permitimos recibir las acciones del teclado
    vistaNivel2->setFocus();
}

void MainWindow::jugar()
{
    // Iniciamos el nivel con la dificultad seleccionada
    if (nivelSeleccionado == 1)
    {
        iniciarNivel1(dificultadSeleccionada);
    }
    else
    {
        iniciarNivel2(dificultadSeleccionada);
    }
}

QPushButton *MainWindow::crearBotonGrafico(
    QWidget *padre, const QString &ruta,
    int x, int y)
{
    // Cargamos la imagen del boton
    QPixmap imagen(ruta);

    // Creamos un boton asociado al menu
    QPushButton *boton = new QPushButton(padre);

    // Utilizamos la imagen como icono
    boton->setIcon(QIcon(imagen));
    boton->setIconSize(imagen.size());

    // Conservamos las dimensiones originales
    boton->setFixedSize(imagen.size());

    // Ubicamos el boton
    boton->move(x, y);

    // Eliminamos la apariencia predeterminada de Qt
    boton->setStyleSheet(
        "QPushButton {"
        "background: transparent;"
        "border: none;"
        "padding: 0px;"
        "}"
        );

    boton->setCursor(Qt::PointingHandCursor);

    return boton;
}

void MainWindow::cambiarNivel()
{
    if (nivelSeleccionado == 1)
    {
        nivelSeleccionado = 2;
        textoNivel->setText("Nivel 2");
    }
    else
    {
        nivelSeleccionado = 1;
        textoNivel->setText("Nivel 1");
    }
}


void MainWindow::cambiarDificultad()
{
    dificultadSeleccionada = !dificultadSeleccionada;

    if (dificultadSeleccionada)
    {
        textoDificultad->setText("Difícil");
    }
    else
    {
        textoDificultad->setText("Normal");
    }
}
//------------------------------ Avanzar el nivel 2 despues de ganar ----------------------//
void MainWindow::avanzarAlNivel2()
{
    iniciarNivel2(dificultadSeleccionada);
}