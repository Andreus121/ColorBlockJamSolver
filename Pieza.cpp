#include <iostream>
#include <string>
#include "Pieza.h"

Pieza::Pieza(u_int8_t id, 
        char color, 
        int ancho,
        int alto,
        int x,
        int y,
        u_int8_t* geometria) {
    this->id = id;
    this->color = color;
    this->ancho = ancho;
    this->alto = alto;
    this->x = x;
    this->y = y;
    /*
    copiar la geometría para que se aloje dentro de la pieza
    */
    this->geometria = new u_int8_t[ancho*alto];
    for(int i= 0; i<ancho*alto; i++){
        this->geometria[i] = geometria[i];
    }
}

// destructor para borrar el puntero de geometria
Pieza::~Pieza() {
    delete[] this->geometria;
}

// metodo usado para debug
void Pieza::imprimirPieza() {
    std::cout << "Pieza ID: " << (int)id << std::endl;
    std::cout << "Color: " << (int)color << std::endl;
    std::cout << "Ancho: " << ancho << std::endl;
    std::cout << "Alto: " << alto << std::endl;
    std::cout << "Posición: (" << x << ", " << y << ")" << std::endl;
    std::cout << "Geometría: " << std::endl;
    for (int i = 0; i < ancho * alto; i+=ancho) {
        for(int j = 0; j < ancho; j++) {
            if(geometria[i + j] == 1) {
                std::cout << color;
            } else {
                std::cout << "-";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}