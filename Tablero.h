#include <iostream>
#include <string>
#include "Pieza.h"

/*
Clase tablero que contendrá la información del espacio de juego
las piezas, compuertas, salidas y paredes
*/
class Tablero{
    public:
    //atributos
    int ancho;
    int alto;
    char** cuadricula;
    Pieza** piezas;
    int cantidadPiezas;

    Tablero(int ancho, 
        int alto, 
        Pieza** piezas,
        int cantidadPiezas);
    ~Tablero();

    void agregarPieza(Pieza pieza);
    //void agregarPiezas(Pieza* piezas);
    
    void imprimirTablero();

    /*
    void agregarSalida();
    void agregarSalidas();
    void agregarCompuerta();
    void agregarCompuertas();
    */
};

