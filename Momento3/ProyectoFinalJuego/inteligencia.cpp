#include "inteligencia.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

Inteligencia::Inteligencia(int numOpciones)
{
    // Validamos la cantidad de estrategias
    if (numOpciones <= 0)
    {
        throw std::invalid_argument(
            "La inteligencia necesita al menos una opcion");
    }

    cantidadOpciones = numOpciones;

    ultimaOpcion = -1;

    // Inicializamos los registros en cero
    for (int i = 0; i < cantidadOpciones; i++)
    {
        intentos.push_back(0);
        aciertos.push_back(0);
    }
}

// Calculamos la proporcion de aciertos
float Inteligencia::calcularEfectividad(int opcion) const
{
    if (intentos[opcion] == 0)
    {
        return 0;
    }

    return static_cast<float>(aciertos[opcion])
           / intentos[opcion];
}

// Seleccionamos una estrategia
int Inteligencia::elegirOpcion()
{
    // Primero probamos las estrategias desconocidas
    for (int i = 0; i < cantidadOpciones; i++)
    {
        if (intentos[i] == 0)
        {
            ultimaOpcion = i;

            return ultimaOpcion;
        }
    }

    // 20% de probabilidad de explorar
    if (rand() % 100 < 20)
    {
        ultimaOpcion = rand() % cantidadOpciones;

        return ultimaOpcion;
    }

    // Buscamos la estrategia mas efectiva
    int mejorOpcion = 0;

    for (int i = 1; i < cantidadOpciones; i++)
    {
        if (calcularEfectividad(i) >
            calcularEfectividad(mejorOpcion))
        {
            mejorOpcion = i;
        }
    }

    ultimaOpcion = mejorOpcion;

    return ultimaOpcion;
}

// Registramos el resultado de la ultima opcion
void Inteligencia::registrarResultado(bool acerto)
{
    registrarResultado(ultimaOpcion, acerto);
}

// Registramos el resultado de una opcion especifica
void Inteligencia::registrarResultado(
    int opcion, bool acerto)
{
    // Verificamos que la opcion sea valida
    if (opcion < 0 || opcion >= cantidadOpciones)
    {
        throw std::out_of_range(
            "Opcion de inteligencia invalida");
    }

    intentos[opcion]++;

    if (acerto)
    {
        aciertos[opcion]++;
    }
    // Prueba temporal del aprendizaje
    std::cout << "\n--- INTELIGENCIA ---" << std::endl;

    std::cout << "Estrategia utilizada: "
              << opcion << std::endl;

    std::cout << "Resultado: "
              << (acerto ? "Acierto" : "Fallo")
              << std::endl;

    for (int i = 0; i < cantidadOpciones; i++)
    {
        std::cout << "Estrategia " << i
                  << " | Intentos: " << intentos[i]
                  << " | Aciertos: " << aciertos[i]
                  << " | Efectividad: "
                  << calcularEfectividad(i) * 100
                  << "%" << std::endl;
    }
}