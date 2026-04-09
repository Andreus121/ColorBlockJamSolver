#pragma once
#include <string>

/*
Clase pieza para realizar el juego de color block jam
almacena datos de una pieza en el tablero
*/
class Pieza {
    public:
    //atributos
    u_int8_t id;
    char color;
    int ancho;
    int alto;
    int x;
    int y;
    u_int8_t* geometria;

    // constructor
    Pieza(u_int8_t id, 
        char color, 
        int ancho,
        int alto,
        int x,
        int y,
        u_int8_t* geometria);
    
    // destructor
    ~Pieza();
    void imprimirPieza(); // debug

};