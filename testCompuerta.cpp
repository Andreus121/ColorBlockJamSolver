#include <iostream>
#include <string>
#include "Compuerta.h"

int main(){
    //para simular el staticdata se usaran valores fijos por este test
    char colorInicial = 'a';
    char colorFinal = 'c';
    int pasos = 2; // cada cuantos pasos se cambia el color de la compuerta

    //creamos la compuerta
    Compuerta* compuerta1 = new Compuerta(0, colorInicial, nullptr);
    std::cout << "Compuerta: " << compuerta1->id << " Color inicial: " << compuerta1->coloractual << std::endl;

    // Simular el resto de pasos
    for (int i = 0; i < 9; i++) {
        compuerta1->actualizarCompuerta(colorInicial, colorFinal, pasos); // li=2, lf=4, pasos=2
        std::cout << "Paso: " << compuerta1->contadorpasos << " Color actual de la compuerta: " << compuerta1->coloractual << std::endl;
    }
    return 0;

    delete compuerta1;
    return 0;
}