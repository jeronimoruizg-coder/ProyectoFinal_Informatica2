#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>

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
    // Selectores del menu
    QComboBox *selectorNivel;
    QComboBox *selectorDificultad;

    // Funciones de navegacion
    void mostrarMenuPrincipal();
    void iniciarNivel1(bool dificultad);
    void iniciarNivel2(bool dificultad);
    void jugar();
};
#endif // MAINWINDOW_H
