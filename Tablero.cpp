#include <iostream>
#include <string>
#include "Tablero.h"

Tablero::Tablero(int ancho, int alto, Pieza** piezas, int cantidadPiezas, Salida** salidas, int cantidadSalidas){
    this->ancho = ancho;
    this->alto = alto;
    this->piezas = piezas;
    this->cantidadPiezas = cantidadPiezas;
    this->salidas = salidas;
    this->cantidadSalidas = cantidadSalidas;
    // crear matriz vacía con las medidas dadas
    this->cuadricula = new char*[ancho];
    for (size_t i = 0; i < ancho; i++){
        cuadricula[i] = new char[alto];
        for (size_t j = 0; j < alto; j++){
            cuadricula[i][j] = ' ';
        }
    }
}

Tablero::~Tablero(){
    for (int i = 0; i < ancho; i++){
        delete[] cuadricula[i];
    }
    delete[] cuadricula;
    // Destruir todas las piezas
    for (int i = 0; i < cantidadPiezas; i++){
        delete piezas[i];
    }
    delete[] piezas;  // Liberar el array de piezas
}

void Tablero::imprimirTablero(){
    for (int i = 0; i < alto; i++){
        for (int j = 0; j < ancho; j++){
            std::cout << cuadricula[j][i];
        }
        std::cout << std::endl;
    }
}

/*
METODOS PARA AÑADIR COSAS AL TABLERO
*/

//------ AÑADIR PAREDES AL TABLERO ------
void Tablero::agregarParedes(char* paredes[]){
    for(int i = 0; i<alto;i++){
        for(int j=0; j<ancho;j++){
            if(paredes[i][j]=='#'){
                cuadricula[i][j] = '#';
            }
        }
    }
}

//------ AÑADIR PIEZAS AL TABLERO ------
// agrega 1 pieza unica al tablero
void Tablero::agregarPieza(Pieza pieza){
    // agregar la pieza a la matriz
    for (int i = 0; i < pieza.ancho*pieza.alto; i+=pieza.ancho) {
        for(int j = 0; j < pieza.ancho; j++) {
            if((int)pieza.geometria[i + j] == 1) {
                this->cuadricula[pieza.y + j][pieza.x + i/pieza.ancho] = pieza.color;
            }
        }
    }
}

// agrega todas las piezas del tablero
void Tablero::agregarPiezas(){
    for(int i=0; i< cantidadPiezas; i++){
        this->agregarPieza(piezas[i][0]);
    }
}

//------ AÑADIR SALIDAS AL TABLERO ------
void Tablero::agregarSalida(){
    for(int i=0; i< cantidadPiezas; i++){
        Salida* salida = salidas[i];
        if(salida->orentacion == 'H'){
            for(int j=0; j<salida->la; j++){
                this->cuadricula[salida->y][salida->x + j] = salida->color;
            }
        }else{
            for(int j=0; j<salida->la; j++){
                this->cuadricula[salida->y + j][salida->x] = salida->color;
            }
        }
    }
}
