#include "entidad.h"

Entidad::Entidad() {}
void Entidad::actualizarposicion(float posicionX, float posicionY){
    posicion_x = posicionX;
    posicion_y = posicionY;
}
void Entidad::obtenerHitbox(){}