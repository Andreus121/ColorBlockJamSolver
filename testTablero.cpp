#include <iostream>
#include <string>
#include "Tablero.h"

int main(){
    // crear piezas
    u_int8_t geometria1[4] = {1, 0, 1, 1}; // Ejemplo de geometría
    Pieza* pieza1 = new Pieza(1, 'a', 2, 2, 1, 1, geometria1);
    u_int8_t geometria2[6] = {1,1,1,0,1,1};
    Pieza* pieza2 = new Pieza(2,'b',2,3,3,3,geometria2);
    u_int8_t geometria3[6] = {1,1,1,1,0,1};
    Pieza* pieza3 = new Pieza(2,'c',3,2,6,0,geometria3);
    
    // Crear array de piezas dinámicamente (en HEAP)
    Pieza** piezas = new Pieza*[3];
    piezas[0] = pieza1;
    piezas[1] = pieza2;
    piezas[2] = pieza3;

    // crear tablero
    Tablero* tablero = new Tablero(10, 10, piezas, 3);

    tablero->imprimirTablero();

    // agregar piezas al tablero
    std::cout <<"Se agrega la pieza L en x: "<< pieza1->x<<" y: "<<pieza1->y <<std::endl;
    tablero->agregarPieza(*pieza1);
    tablero->imprimirTablero();
    std::cout <<"Se agrega la pieza [ en x: "<< pieza2->x<<" y: "<<pieza2->y <<std::endl;
    tablero->agregarPieza(*pieza2);
    tablero->imprimirTablero();
    std::cout <<"Se agrega la pieza3 en x: "<< pieza2->x<<" y: "<<pieza2->y <<std::endl;
    tablero->agregarPieza(*pieza3);
    tablero->imprimirTablero();


    delete tablero;  // Libera cuadricula Y el array de piezas
    return 0;
}

