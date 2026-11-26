#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "vistanivel1.h"
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QMessageBox>

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
    // Creamos el contenedor principal
    QWidget *menu = new QWidget(this);

    // Organizamos los elementos verticalmente
    QVBoxLayout *distribucion = new QVBoxLayout(menu);

    // Titulo del juego
    QLabel *titulo = new QLabel(
        "EL LABORATORIO BOTANICO DE DEXTER", menu);

    titulo->setAlignment(Qt::AlignCenter);

    titulo->setFont(
        QFont("Consolas", 28, QFont::Bold));

    // Seleccion del nivel
    QLabel *textoNivel = new QLabel(
        "Selecciona el nivel", menu);

    selectorNivel = new QComboBox(menu);

    selectorNivel->addItem("Nivel 1");
    selectorNivel->addItem("Nivel 2");

    // Seleccion de dificultad
    QLabel *textoDificultad = new QLabel(
        "Selecciona la dificultad", menu);

    selectorDificultad = new QComboBox(menu);

    selectorDificultad->addItem("Normal");
    selectorDificultad->addItem("Dificil");

    // Botones del menu
    QPushButton *botonJugar =
        new QPushButton("JUGAR", menu);

    QPushButton *botonSalir =
        new QPushButton("SALIR", menu);

    // Organizamos los elementos
    distribucion->addStretch();

    distribucion->addWidget(titulo);
    distribucion->addSpacing(40);

    distribucion->addWidget(textoNivel);
    distribucion->addWidget(selectorNivel);

    distribucion->addSpacing(15);

    distribucion->addWidget(textoDificultad);
    distribucion->addWidget(selectorDificultad);

    distribucion->addSpacing(30);

    distribucion->addWidget(botonJugar);
    distribucion->addWidget(botonSalir);

    distribucion->addStretch();

    // Limitamos el ancho de los controles
    textoNivel->setFixedWidth(300);
    selectorNivel->setFixedWidth(300);

    textoDificultad->setFixedWidth(300);
    selectorDificultad->setFixedWidth(300);

    botonJugar->setFixedSize(300, 50);
    botonSalir->setFixedSize(300, 50);

    // Centramos los controles
    distribucion->setAlignment(
        textoNivel, Qt::AlignHCenter);

    distribucion->setAlignment(
        selectorNivel, Qt::AlignHCenter);

    distribucion->setAlignment(
        textoDificultad, Qt::AlignHCenter);

    distribucion->setAlignment(
        selectorDificultad, Qt::AlignHCenter);

    distribucion->setAlignment(
        botonJugar, Qt::AlignHCenter);

    distribucion->setAlignment(
        botonSalir, Qt::AlignHCenter);

    // Conectamos el boton salir
    connect(botonSalir, &QPushButton::clicked,
            this, &MainWindow::close);

    // Conectamos el boton jugar
    connect(botonJugar, &QPushButton::clicked,
            this, &MainWindow::jugar);

    // Mostramos el menu
    setCentralWidget(menu);
}

void MainWindow::iniciarNivel1(bool dificultad)
{
    // Creamos el nivel con la dificultad seleccionada
    VistaNivel1 *vistaNivel1 =
        new VistaNivel1(dificultad);

    // Mostramos el nivel
    setCentralWidget(vistaNivel1);

    // Permitimos recibir las acciones del teclado
    vistaNivel1->setFocus();
}

void MainWindow::jugar()
{
    // Consultamos la dificultad seleccionada
    bool dificultad =
        selectorDificultad->currentIndex() == 1;

    // Consultamos el nivel seleccionado
    if (selectorNivel->currentIndex() == 0)
    {
        iniciarNivel1(dificultad);
    }
    else
    {
        QMessageBox::information(
            this,
            "Nivel 2",
            "Nivel pendiente de integracion");
    }
}