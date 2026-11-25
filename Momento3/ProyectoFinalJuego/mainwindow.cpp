#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "vistanivel1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    VistaNivel1 *vistaNivel1 = new VistaNivel1();
    setCentralWidget(vistaNivel1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
