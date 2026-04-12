#include <iostream>
#include <string>

/*
Clase que contendrá la información de cada salida de las piezas
en el tablero, como su tamaño inicial y final, 
cuantos espacios varía su tamaño (pasos), etc
*/
class Salida{
    public:
    char color;
    // coordendas de la salida en el tablero
    int x;
    int y;
    // indica si la salida es vertical u horizontal
    char orentacion;
    int li;// tamaño minimo e inicial de la salida
    int lf;// tamaño maximo de la salida
    int la;;// tamaño actual de la salida
    int pasos;// cuantos espacios va a variar su tamaño entre li y lf
    // siempre empieza en 0 (indica que le toca crecer) 1 ind
    char direccion;// indica si a la salida le toca crecer o achicar

    //constructor de una pieza que no varía su tamaño
    Salida(char color,
        int x,
        int y,
        char orentacion,
        int la);
    
    //constructor de una pieza que varía su tamaño
    Salida(char color,
        int x,
        int y,
        char orentacion,
        int li,
        int lf,
        int pasos);
    
    // función que actualiza el tamaño de la salida dependiendo de su dirección
    void actualizarSalida();
};