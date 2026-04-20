#include <iostream>
#include "Movimiento.h"

int main(){
    // movimiento por defecto
    Movimiento m0;
    std::cout << "Movimiento por defecto: id=" << (int)m0.idBloque
              << " dir=" << m0.direccion
              << " celdas=" << m0.celdas << std::endl;
    
    // movimiento con valores
    Movimiento m1(3, 'R', 5);
    std::cout << "Movimiento: id=" << (int)m1.idBloque<< " dir=" << m1.direccion<< " celdas=" << m1.celdas << std::endl;
    return 0;
}