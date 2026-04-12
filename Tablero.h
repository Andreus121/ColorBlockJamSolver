#include <iostream>
#include <string>
#include "Pieza.h"
#include "Salida.h"

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
    Salida** salidas;
    int cantidadSalidas;

    Tablero(int ancho, 
        int alto, 
        Pieza** piezas,
        int cantidadPiezas,
        Salida** salidas,
        int cantidadSalidas);
    ~Tablero();
    void imprimirTablero();
    
    //añadir las paredes al tablero
    void agregarParedes(char* paredes[]);

    //añadir las piezas al juego
    void agregarPieza(Pieza pieza);
    void agregarPiezas();

    void agregarSalida();
    //void agregarSalidas();

    /*
    void agregarCompuerta();
    void agregarCompuertas();
    */
};

