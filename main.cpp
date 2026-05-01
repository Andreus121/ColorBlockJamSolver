#include <iostream>
#include <cstring>
#include "Parser.h"
#include "Tablero.h"
#include "Aestrella.h"
#include "Camino.h"

/*
Programa principal de Color Block Jam.
Presenta un menu con 5 opciones:
1) Cargar archivo de configuracion
2) Resolver el juego (corre A* y muestra la secuencia + tiempo)
3) Aplicar secuencia manual de movimientos
4) Salir

Mantiene en memoria entre opciones:
- staticData y los arreglos iniciales (bloques/salidas/compuertas) parseados
- el ultimo Camino solucion encontrado por A*
*/

//helper: libera todos los recursos asociados a un staticData parseado.
//Lo hago aqui aparte porque se usa en varios lugares (al cargar nuevo, al salir).
void liberarStaticData(StaticData* sd){
    if(sd == nullptr) return;
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
    delete[] sd->xSalidas; 
    delete[] sd->ySalidas; 
    delete[] sd->coloresSalidas;
    delete[] sd->orientacionSalidas; 
    delete[] sd->largoISalidas;
    delete[] sd->largoFSalidas; 
    delete[] sd->pasosSalidas;
    delete[] sd->xCompuertas; 
    delete[] sd->yCompuertas;
    delete[] sd->largoCompuertas;
    delete[] sd->orientacionCompuertas;
    delete[] sd->colorICompuertas; 
    delete[] sd->colorFCompuertas;
    delete[] sd->pasosCompuertas;
    delete sd;
}

//Crea un Tablero "fresco" recargando el archivo desde disco.
//Necesario para opciones 3 y 4: cada Tablero se modifica al moverBloque,
//asi que para mostrar la solucion paso a paso necesitamos uno limpio.
//Devuelve nullptr en caso de error. Si retorna no-null, sdSalida apunta
//al staticData asociado (que el llamador debe liberar despues).
Tablero* recargarTableroDesdeArchivo(const char* ruta, StaticData*& sdSalida){
    Parser* p = new Parser();
    if(!p->cargar(ruta)){
        delete p;
        sdSalida = nullptr;
        return nullptr;
    }
    Tablero* t = new Tablero(p->staticData, p->bloques, p->salidas, p->compuertas);
    sdSalida = p->staticData;
    delete p;
    return t;
}

void mostrarMenu(){
    std::cout << "" << std::endl;
    std::cout << "  COLOR BLOCK JAM" << std::endl;
    std::cout << "1) Cargar archivo de configuracion" << std::endl;
    std::cout << "2) Resolver el juego con A* (muestra tablero paso a paso)" << std::endl;
    std::cout << "3) Aplicar secuencia manual de movimientos" << std::endl;
    std::cout << "4) Salir" << std::endl;
    std::cout << "" << std::endl;
    std::cout << "Opcion: ";
}

int main(){
    //estado del programa que se mantiene entre opciones
    char nombreArchivo[512] = {0};       //nombre del archivo cargado actualmente
    StaticData* staticData = nullptr;  //datos estaticos del archivo cargado
    Tablero* tableroInicial = nullptr; //tablero en su estado de inicio
    Camino* ultimaSolucion = nullptr;  //ultimo camino encontrado por A* (puede ser nullptr)
    int ultimoTiempoMs = 0;            //tiempo de la ultima resolucion

    bool salir = false;
    while(!salir){
        mostrarMenu();
        int opcion;
        if(!(std::cin >> opcion)){
            std::cin.clear();
            //descartar resto de la linea
            char basura[256];
            std::cin.getline(basura, sizeof(basura));
            std::cout << "Entrada invalida." << std::endl;
            continue;
        }
        //limpiar el resto de la linea (el \n que dejo el >>)
        char basura[256];
        std::cin.getline(basura, sizeof(basura));

        switch(opcion){
            case 1: {
                //Cargar archivo
                std::cout << "Ingrese el nombre del archivo: ";
                char ruta[256];
                std::cin.getline(ruta, sizeof(ruta));
                if(std::strlen(ruta) == 0){
                    std::cout << "Archivo no encontrado" << std::endl;
                    break;
                }

                //leer el archivo con el parser
                Parser* p = new Parser();
                if(!p->cargar(ruta)){
                    std::cout << "No se pudo cargar el archivo." << std::endl;
                    delete p;
                    break;
                }

                //liberar estado anterior (si habia)
                if(ultimaSolucion){
                    delete ultimaSolucion; ultimaSolucion = nullptr;
                }
                if(tableroInicial){
                    delete tableroInicial; tableroInicial = nullptr;
                }
                if(staticData){
                    liberarStaticData(staticData); staticData = nullptr;
                }

                //Guardar datos nuevos
                staticData = p->staticData;
                tableroInicial = new Tablero(p->staticData, p->bloques, p->salidas, p->compuertas);
                std::strncpy(nombreArchivo, ruta, sizeof(nombreArchivo) - 1);
                nombreArchivo[sizeof(nombreArchivo) - 1] = '\0';
                delete p;

                std::cout << "\nArchivo cargado: " << nombreArchivo << std::endl;
                std::cout << "Dimensiones: " << staticData->anchoTablero
                          << "x" << staticData->altoTablero << std::endl;
                std::cout << "Bloques: " << staticData->cantidadBloques
                          << ", Salidas: " << staticData->cantidadSalidas
                          << ", Compuertas: " << staticData->cantidadCompuertas << std::endl;
                std::cout << "Limite de movimientos: " << staticData->limiteMovimientos << std::endl;
                std::cout << "\nEstado inicial:" << std::endl;
                tableroInicial->imprimir();
                break;
            }

            case 2: {
                //Resolver con A* y, si hay solucion, mostrar tablero paso a paso
                if(tableroInicial == nullptr){
                    std::cout << "Primero carga un archivo desde la opcion 1" << std::endl;
                    break;
                }

                //limpiar solucion anterior si la habia
                if(ultimaSolucion){
                    delete ultimaSolucion; ultimaSolucion = nullptr;
                }

                std::cout << "\nResolviendo..." << std::endl;
                Aestrella* astar = new Aestrella(tableroInicial);
                Camino* sol = astar->resolver();
                ultimoTiempoMs = astar->tiempoMs();
                int explorados = astar->estadosExplorados;
                int generados = astar->estadosGenerados;
                delete astar;

                if(sol == nullptr){
                    std::cout << "Tiempo resolucion: " << ultimoTiempoMs << " [mseg]" << std::endl;
                    std::cout << "Estados explorados: " << explorados << std::endl;
                    std::cout << "Estados generados: " << generados << std::endl;
                    std::cout << "No tiene solucion" << std::endl;
                    break;
                }

                //hay solucion: la guardamos y mostramos paso a paso
                ultimaSolucion = sol;

                //recargamos el archivo para tener un tablero fresco que ir modificando
                StaticData* sdFresco = nullptr;
                Tablero* fresco = recargarTableroDesdeArchivo(nombreArchivo, sdFresco);
                if(fresco == nullptr){
                    std::cout << "Error: no se pudo recargar el archivo para mostrar paso a paso." << std::endl;
                    //al menos mostramos la cadena
                    std::cout << "Pasos: ";
                    sol->imprimir();
                    break;
                }

                std::cout << "\nReproduciendo solucion paso a paso" << std::endl;
                sol->aplicar(fresco, true);

                delete fresco;
                liberarStaticData(sdFresco);

                //resumen final
                std::cout << "\nResumen" << std::endl;
                std::cout << "Tiempo resolucion: " << ultimoTiempoMs << " [mseg]" << std::endl;
                std::cout << "Estados explorados: " << explorados << std::endl;
                std::cout << "Estados generados: " << generados << std::endl;
                std::cout << "Solucion encontrada, pasos:" << std::endl;
                sol->imprimir();
                break;
            }

            case 3: {
                //Aplicar secuencia manual ingresada por el usuario
                if(tableroInicial == nullptr){
                    std::cout << "Primero carga un archivo desde la opcion 1" << std::endl;
                    break;
                }

                std::cout << "Ingrese secuencia (ej: R1,2D2,1R1,10): ";
                char linea[1024];
                std::cin.getline(linea, sizeof(linea));
                if(std::strlen(linea) == 0){
                    std::cout << "Secuencia vacia, no se hace nada" << std::endl;
                    break;
                }

                //parsear la secuencia a un Camino
                Camino* manual = new Camino();
                if(!manual->parsearDesdeString(linea)){
                    std::cout << "Formato invalido" << std::endl;
                    delete manual;
                    break;
                }

                //recargar el archivo para tener un tablero fresco
                StaticData* sdFresco = nullptr;
                Tablero* fresco = recargarTableroDesdeArchivo(nombreArchivo, sdFresco);
                if(fresco == nullptr){
                    std::cout << "Error: no se pudo recargar el archivo." << std::endl;
                    delete manual;
                    break;
                }

                std::cout << "\nAplicando secuencia" << std::endl;
                manual->aplicar(fresco, true);

                delete manual;
                delete fresco;
                liberarStaticData(sdFresco);
                break;
            }

            case 4: {
                salir = true;
                break;
            }

            default: {
                std::cout << "Opcion invalida, ingresa numeros del 1 al 4." << std::endl;
                break;
            }
        }
    }

    //limpieza final
    if(ultimaSolucion) delete ultimaSolucion;
    if(tableroInicial) delete tableroInicial;
    if(staticData) liberarStaticData(staticData);

    std::cout << "Bye :)" << std::endl;
    return 0;
}