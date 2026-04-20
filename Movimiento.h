#pragma once
#include <cstdint>

/*
Representa el movimiento de un bloque
Sirve para reconstruir el camino de la solución y para
*/
class Movimiento {
    public:
    uint8_t idBloque;   // id del bloque movido
    char direccion;     // 'U' arriba, 'D' abajo, 'L' izquierda, 'R' derecha
    int celdas;         // cantidad de celdas deslizadas

    Movimiento();
    Movimiento(uint8_t idBloque, char direccion, int celdas);
};