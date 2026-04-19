#pragma once
#include <iostream>
#include <string>
#include "StaticData.h"
#include "Bloque.h"
#include "Salida.h"
#include "Compuerta.h"

class Tablero {
    public:
    //atributos
    StaticData* staticData; // datos del tablero, bloques, salidas y compuertas
    int cantidadBloques;
    Bloque** bloques;  // arreglo de punteros a bloques (solo los que quedan en juego en este turno)
    Salida** salidas;  // arreglo de punteros a salidas
    Compuerta** compuertas; // arreglo de compuertas
    int contadorMovimientos; // contador de movimientos realizados en el turno actual (inicia en 0)
    

    //constructor
    Tablero(StaticData* staticData,
        Bloque** bloques,
        Salida** salidas,
        Compuerta** compuertas);

    //métodos

};