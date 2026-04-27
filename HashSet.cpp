#include "HashSet.h"

//constructor
HashSet::HashSet(int capacidadInicial){
    if(capacidadInicial < 1) capacidadInicial = 1;
    this->capacidad = capacidadInicial;
    this->cantidad = 0;
    this->lista = new Nodo*[this->capacidad];
    for(int i = 0; i < this->capacidad; i++) this->lista[i] = nullptr;
}

//destructor
HashSet::~HashSet(){
    //liberamos todos los Tablero y todos los Nodo
    for(int i = 0; i < this->capacidad; i++){
        Nodo* actual = this->lista[i];
        while(actual != nullptr){
            Nodo* sig = actual->siguiente;
            delete actual->tablero;
            delete actual;
            actual = sig;
        }
    }
    delete[] this->lista;
}

//verificar si existe un tablero igual
bool HashSet::contiene(Tablero* tablero){
    unsigned long h = tablero->hash();
    int idx = (int)(h % (unsigned long)this->capacidad);

    Nodo* actual = this->lista[idx];
    while(actual != nullptr){
        if(actual->tablero->esIgual(tablero)) return true;
        actual = actual->siguiente;
    }
    return false;
}

//insertar un tablero al Hash
bool HashSet::insertar(Tablero* tablero){
    //si ya hay uno igual, no insertar
    if(contiene(tablero)) return false;

    //si el factor de carga supera 0.75, rehashear ANTES de insertar
    //esto mantiene las listas cortas y las operaciones en O(1) promedio
    if(this->cantidad * 4 >= this->capacidad * 3){
        rehashear();
    }

    unsigned long h = tablero->hash();
    int idx = (int)(h % (unsigned long)this->capacidad);

    //insertar al inicio de la lista del bucket (O(1))
    Nodo* nuevo = new Nodo;
    nuevo->tablero = tablero;
    nuevo->siguiente = this->lista[idx];
    this->lista[idx] = nuevo;
    this->cantidad++;
    return true;
}

//ver cuantos tableros hay
int HashSet::tamano(){
    return this->cantidad;
}

//aumentar el tamaño del hash
void HashSet::rehashear(){
    int nuevoLargo = this->capacidad * 2 + 1; //+1 para mantener imparidad

    //crear nuevos lista
    Nodo** nuevos = new Nodo*[nuevoLargo];
    for(int i = 0; i < nuevoLargo; i++) nuevos[i] = nullptr;

    //mover todos los nodos existentes al nuevo hash
    //reutilizamos los nodos (no creamos nuevos) para evitar allocs
    for(int i = 0; i < this->capacidad; i++){
        Nodo* actual = this->lista[i];
        while(actual != nullptr){
            Nodo* sig = actual->siguiente;
            //recalcular indice con el nuevo numero de lista
            unsigned long h = actual->tablero->hash();
            int nuevoIdx = (int)(h % (unsigned long)nuevoLargo);
            //insertar al inicio del nuevo bucket
            actual->siguiente = nuevos[nuevoIdx];
            nuevos[nuevoIdx] = actual;
            actual = sig;
        }
    }

    //liberar el arreglo viejo (pero NO los nodos, que los reubicamos)
    delete[] this->lista;
    this->lista = nuevos;
    this->capacidad = nuevoLargo;
}