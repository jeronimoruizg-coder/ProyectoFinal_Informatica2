#ifndef INTELIGENCIA_H
#define INTELIGENCIA_H

#include <vector>

class Inteligencia
{
private:

    // Cantidad de estrategias disponibles
    int cantidadOpciones;

    // Historial de cada estrategia
    std::vector<int> intentos;
    std::vector<int> aciertos;

    // Ultima estrategia seleccionada
    int ultimaOpcion;

    // Calcula la efectividad de una estrategia
    float calcularEfectividad(int opcion) const;

public:

    Inteligencia(int numOpciones);

    int elegirOpcion();

    void registrarResultado(bool acerto);

    // Permite evaluar una estrategia especifica
    void registrarResultado(int opcion, bool acerto);
};

#endif // INTELIGENCIA_H