#include <iostream>
#include <string>
#include "Bloque.h"

int main() {
    // Crear una pieza de ejemplo
    Bloque* bloque1 = new Bloque(1, 2, 3,nullptr);

    // Imprimir los datos del bloque
    std::cout << "Bloque: " << (int)bloque1->id << ", Posición: (" << bloque1->x << ", " << bloque1->y << ")" << std::endl;
    
    delete bloque1;
    return 0;
}