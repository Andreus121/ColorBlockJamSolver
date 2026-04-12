#include <iostream>
#include "Salida.h"

// Constructor para piezas que no varían su tamaño
Salida::Salida(char color,
        int x,
        int y,
        char orentacion,
        int la){
    this->color = color;
    this->x = x;
    this->y = y;
    this->orentacion = orentacion;
    this->li = la;
    this->lf = la;
    this->la = la;
    this->pasos = 0;
    this->direccion = '0';
}
// Constructor para piezas que varían su tamaño
Salida::Salida(char color,
        int x,
        int y,
        char orentacion,
         int li,
         int lf,
         int pasos){
    this->color = color;
    this->x = x;
    this->y = y;
    this->orentacion = orentacion;
    this->li = li;
    this->lf = lf;
    this->la = li;
    this->pasos = pasos;
    this->direccion = '0';
}

/*
actualiza el tamaño de la salida dependiendo de su dirección
y cabia la dirección si es necesario
*/
void Salida::actualizarSalida(){
    if(this->direccion == '0'){// si le toca crecer
        this->la += this->pasos;
        if(this->la >= this->lf){// si llega a su tamaño maximo
            this->la = this->lf;
            this->direccion = '1';
        }
    }else{// si le toca achicar
        this->la -= this->pasos;
        if(this->la <= this->li){// si llega a su tamaño minimo
            this->la = this->li;
            this->direccion = '0';
        }
    }
}