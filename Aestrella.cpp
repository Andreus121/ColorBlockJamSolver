#include <iostream>
#include <ctime>//libreria para medir el tiempo de CPU
#include "Aestrella.h"

Aestrella::Aestrella(Tablero* tableroInicial){
    this->inicial = tableroInicial;
    this->ultimoTiempo = 0;
    this->estadosExplorados = 0;
    this->estadosGenerados = 0;
}

Aestrella::~Aestrella(){
    //no liberamos 'inicial': no somos dueños
}

int Aestrella::tiempoMs(){
    return this->ultimoTiempo;
}

//el algoritmo A* en si
Camino* Aestrella::resolver(){
    //iniciar el cronometro
    clock_t inicioTiempo = clock();//guardar el tiempo en el que inició

    //datos para comparar en el resultado y ver la eficiencia
    this->estadosExplorados = 0;
    this->estadosGenerados = 0;

    //OpenSet y ClosedSet
    MinHeap* openSet = new MinHeap(256);//arreglo de prioridad
    HashSet* closedSet = new HashSet(1021);//hash con un primo grande

    //clonamos el tablero inicial para que el OpenSet/ClosedSet tengan su propio tablero inicial
    //el tablero inicial recibido se hizo en otro lado, no soy dueño de el
    Tablero* raiz = this->inicial->clonar();
    raiz->padre = nullptr; //la raiz no tiene padre en el arbol de busqueda
    int h0 = raiz->heuristica();//calcular su heuristica para la prioridad inicial
    openSet->push(raiz, 0 + h0);//f = g + h, g=0 en la raiz

    //obtener el limite de movimientos inicial
    int stepLimit = this->inicial->staticData->limiteMovimientos;

    //si se necesitan más pasos que el limite del mapa, se da por imposible y finaliza
    int cantMinMovimientos = raiz->contarMovimientosNecesarios();
    if(cantMinMovimientos > stepLimit){
        delete raiz;
        delete openSet;
        delete closedSet;
        clock_t finTiempo = clock();
        this->ultimoTiempo = (int)((double)(finTiempo - inicioTiempo) * 1000.0 / CLOCKS_PER_SEC);
        return nullptr; //sin solucion
    }

    Camino* resultado = nullptr;//almacenar el camino resultante
    Tablero* tableroSolucion = nullptr;//guardar el tablero final para la reconstruccion del camino

    //comienzo del algoritmo A*
    while(!openSet->vacio()){//mientras openSet no este vacio
        //sacar el tablero con mayor prioridad
        Tablero* actual = openSet->pop();

        //verificar en el closedSet si ya fue explorado
        //si lo exploré lo salto
        if(closedSet->contiene(actual)){
            delete actual;
            continue;
        }

        //sumo un estado explorado al contador
        this->estadosExplorados++;

        //verificar si es solucion
        if(actual->esSolucion()){
            tableroSolucion = actual; //guardar para reconstruir camino
            //no se guarda en closedSet ya que es el final
            break;//termina el ciclo
        }

        //podar, si a supera los pasos limite, no hay que expandirlo
        //Si g == stepLimit podriamos estar justo en el limite
        //como no vamos a expandir mas (los vecinos tendrian g+1 > stepLimit), saltamos.
        if(actual->contadorMovimientos >= stepLimit){
            //este tablero queda en el closedSet para que no lo exploren
            closedSet->insertar(actual);//guardar en closedSet
            actual->liberarCuadricula();//destruir la cuadricula para ahorrar memoria
            continue;
        }

        //generar vecinos, esto ya reconstruye cuadriculas de los hijos
        int numeroVecinos;
        Tablero** vecinos = actual->generarVecinos(numeroVecinos);
        this->estadosGenerados += numeroVecinos;

        //insertar actual al closedSet
        closedSet->insertar(actual);
        //liberar la cuadricula de actual
        actual->liberarCuadricula();

        //encolar vecinos que no esten en closedSet
        for(int i = 0; i < numeroVecinos; i++){
            Tablero* v = vecinos[i];//tomar un vecino
            if(closedSet->contiene(v)){//verificar que no haya sido explorado
                delete v; //borrar si lo exploramos
                continue;
            }
            //calcular la heuristica de cada vecino
            int g = v->contadorMovimientos;
            int h = v->heuristica();
            openSet->push(v, g + h);
        }
        delete[] vecinos;//borrar el buffer de vecinos
        //terminar el algoritmo
    }

    //si se encontro solucion, reconstruir el camino
    if(tableroSolucion != nullptr){
        resultado = reconstruirCamino(tableroSolucion);
    }

    //LIMPIEZA: vaciar lo que queda en OpenSet (tableros encolados no explorados)
    while(!openSet->vacio()){
        Tablero* t = openSet->pop();
        //puede que ya este en closedSet (duplicado); si no, hay que liberarlo.
        //ya que el closed set se encargará de liberarlo después
        if(!closedSet->contiene(t)){
            delete t;
        }
    }
    
    delete openSet;

    //LIMPIEZA: ya se reconstruyó el camino así que no necesitamos los
    //estados usados, borramos la solucion (si es que hay)
    //y borramos todos los tableros de closedSet
    if(tableroSolucion != nullptr){
        delete tableroSolucion;
    }
    delete closedSet;


    clock_t finTiempo = clock();//guardar el tiempo en donde termina
    double ms = (double)(finTiempo - inicioTiempo) * 1000.0 / CLOCKS_PER_SEC;//calcular diferencia de tiempo
    this->ultimoTiempo = (int)ms;//guardar el resultado

    return resultado;//retornar el camino
}

//Reconstruye el camino solucion siguiendo padres desde 'solucion' hasta el inicial
//Los movimientos se van guardando en orden inverso, y al final se dan vuelta
//para que queden del primero al ultimo
Camino* Aestrella::reconstruirCamino(Tablero* solucion){
    //contar cuantos movimientos hay en la cadena
    int n = 0;
    Tablero* tablero = solucion;
    while(tablero->padre != nullptr){
        n++;
        tablero = tablero->padre;
    }

    //crear camino con capacidad suficiente
    Camino* camino;
    if(n == 0){
        //caso borde: solucion ya es el inicial, no hay movimientos
        camino = new Camino(1);
    } else {
        camino = new Camino(n);//camino con n movimientos
    }

    //recolectar movimientos en orden inverso
    Movimiento* inverso = new Movimiento[n];
    int idx = 0;
    tablero = solucion;  //reiniciar el tablero para este otro while
    while(tablero->padre != nullptr){
        inverso[idx] = tablero->movimientoOrigen;//guardar el movimiento
        idx++;
        tablero = tablero->padre;//retroceder al tablero anterior
    }

    //pasar al camino en orden directo
    for(int i = n - 1; i >= 0; i--){
        camino->agregar(inverso[i]);
    }
    delete[] inverso;//borrar el buffer temporal
    return camino;
}