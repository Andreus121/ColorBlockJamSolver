#include "Movimiento.h"

Movimiento::Movimiento(){
    this->idBloque = 0;
    this->direccion = ' ';
    this->celdas = 0;
}
Movimiento::Movimiento(uint8_t idBloque, char direccion, int celdas){
    this->idBloque = idBloque;
    this->direccion = direccion;
    this->celdas = celdas;
}