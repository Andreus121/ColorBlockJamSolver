#pragma once
#include "Tablero.h"
#include "Camino.h"
#include "MinHeap.h"
#include "HashSet.h"

/*
Aestrella: algoritmo de busqueda informada para resolver Color Block Jam
Usa MinHeap como OpenSet, cola de prioridad O(log n) con los estados por explorar
HashSet como ClosedSet, detecta estados repetidos ya visitados en O(1) promedio

Cada estado es un Tablero. La prioridad de cada estado es f = g + h:
- g = contadorMovimientos del tablero (celdas deslizadas hasta ahora)
- h = heuristica() del tablero (estimacion de distancia al final)

El algoritmo:
1) Meter el tablero inicial al OpenSet con prioridad f = h(inicial)
2) Mientras OpenSet no este vacio
   a) Sacar el tablero con menor f. Llamarlo "actual"
   b) Si ya estaba en ClosedSet, saltarlo (entrada obsoleta)
   c) Si actual es solucion, reconstruir el camino y terminar
   d) Si g(actual) >= STEP_LIMIT, no vale la pena expandir (poda)
   e) Meter actual al ClosedSet, liberar su cuadricula (ahorro memoria)
   f) Generar vecinos de actual, ara cada vecino
      - si ya esta en ClosedSet, liberarlo y continuar
      - si no, meterlo al OpenSet con prioridad f(vecino)
3) Si OpenSet se vacio sin encontrar solucion: no hay solucion

Uso:
  Aestrella* a = new Aestrella(tableroInicial);
  Camino* sol = a->resolver();// nullptr si no hay solucion
  int ms = a->tiempoMs();
  delete a;
  delete sol;
*/
class Aestrella {
    public:
    Tablero* inicial;//tablero inicial (NO es dueño, solo referencia)
    int ultimoTiempo;//tiempo que tomo la ultima llamada a resolver()
    int estadosExplorados;//para estadisticas
    int estadosGenerados;//para estadisticas

    Aestrella(Tablero* tableroInicial);//constructor, recibe el puntero del tablero inicial
    ~Aestrella();//destructor

    //Busca una solucion, retorna nullptr si no la hay
    //El Camino retornado es dueño de sus movimientos; llamador debe hacer delete
    Camino* resolver();

    //Tiempo en milisegundos de la ultima ejecucion de resolver()
    int tiempoMs();

    private:
    //Arma un Camino siguiendo los punteros padre desde el tablero solucion
    //hasta el inicial.
    Camino* reconstruirCamino(Tablero* solucion);
};