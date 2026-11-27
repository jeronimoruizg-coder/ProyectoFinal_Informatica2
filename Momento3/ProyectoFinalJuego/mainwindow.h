#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    // Opciones seleccionadas en el menu
    int nivelSeleccionado;
    bool dificultadSeleccionada;

    // Textos que muestran las opciones actuales
    QLabel *textoNivel;
    QLabel *textoDificultad;

    void cambiarNivel();
    void cambiarDificultad();
    QPushButton *crearBotonGrafico(
        QWidget *padre, const QString &ruta,
        int x, int y);

    // Funciones de navegacion
    void mostrarMenuPrincipal();
    void iniciarNivel1(bool dificultad);
    void iniciarNivel2(bool dificultad);
    void jugar();
};
#endif // MAINWINDOW_H
