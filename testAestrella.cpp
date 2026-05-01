#include <iostream>
#include "Parser.h"
#include "Tablero.h"
#include "Aestrella.h"

//helper para limpiar staticData
void liberarStaticData(StaticData* sd){
    if(sd->paredesTablero){
        for(int i = 0; i < sd->altoTablero; i++) delete[] sd->paredesTablero[i];
        delete[] sd->paredesTablero;
    }
    delete[] sd->coloresBloques;
    delete[] sd->anchoBloques;
    delete[] sd->altoBloques;
    if(sd->geometriaBloques){
        for(int i = 0; i < sd->cantidadBloques; i++) delete[] sd->geometriaBloques[i];
        delete[] sd->geometriaBloques;
    }
    delete[] sd->xSalidas; delete[] sd->ySalidas; delete[] sd->coloresSalidas;
    delete[] sd->orientacionSalidas; delete[] sd->largoISalidas;
    delete[] sd->largoFSalidas; delete[] sd->pasosSalidas;
    delete[] sd->xCompuertas; delete[] sd->yCompuertas;
    delete[] sd->colorICompuertas; delete[] sd->colorFCompuertas;
    delete[] sd->pasosCompuertas;
    delete sd;
}

void probar(const char* ruta){
    std::cout << "====================================" << std::endl;
    std::cout << "Resolviendo: " << ruta << std::endl;
    std::cout << "====================================" << std::endl;

    Parser* p = new Parser();
    if(!p->cargar(ruta)){
        std::cout << "No se pudo cargar " << ruta << std::endl;
        delete p;
        return;
    }
    Tablero* inicial = new Tablero(p->staticData, p->bloques, p->salidas, p->compuertas);
    StaticData* sd = inicial->staticData;
    delete p;

    std::cout << "Estado inicial:" << std::endl;
    inicial->imprimir();

    Aestrella* astar = new Aestrella(inicial);
    Camino* sol = astar->resolver();

    std::cout << "\nEstados explorados: " << astar->estadosExplorados << std::endl;
    std::cout << "Estados generados:  " << astar->estadosGenerados << std::endl;
    std::cout << "Tiempo: " << astar->tiempoMs() << " ms" << std::endl;

    if(sol == nullptr){
        std::cout << "Juego sin solucion." << std::endl;
    } else {
        std::cout << "Solucion encontrada. Pasos:" << std::endl;
        sol->imprimir();
        std::cout << "Total movimientos (expandidos): " << sol->cantidad << std::endl;
        delete sol;
    }

    delete astar;
    delete inicial;
    liberarStaticData(sd);
    std::cout << std::endl;
}

int main(int argc, char** argv){
    if(argc >= 2){
        probar(argv[1]);
    } else {
        probar("compuerta1.txt");
        //probar("facil2.txt");
        //probar("Ejemplo3.txt");
        //probar("Ejemplo4.txt");
    }
    return 0;
}