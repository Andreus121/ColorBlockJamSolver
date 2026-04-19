#include <iostream>
#include <string>
#include "Salida.h"

int main(){
    //para simular el staticdata se usaran valores fijos por este test
    int li = 2; // largo inicial
    int lf = 4; // largo final
    int pasos = 2; // cada cuantos pasos se cambia el tamaño de la salida
    Salida* salida1 = new Salida(0, li, nullptr);
    
    //datos iniciales de la salida
    std::cout << "Salida: " << salida1->id << ", Tamaño actual: " << salida1->la << std::endl;
    //datos primer paso
    std::cout << "Paso " << salida1->contadorpasos << ": Tamaño actual de la salida: " << salida1->la << std::endl;

    // Simular el resto de pasos
    for (int i = 0; i < 9; i++) {
        salida1->actualizarSalida(li, lf, pasos); // li=2, lf=4, pasos=2
        std::cout << "Paso " << salida1->contadorpasos << ": Tamaño actual de la salida: " << salida1->la << std::endl;
    }

    delete salida1;
    return 0;
}