#include <iostream>
#include "Tablero.h"
 
int main(){
    //SIMPLE1 del enunciado: 8x8, bloque 2x2 color 'a' en (4,4), salida V en (7,4) largo 2
    StaticData* sd = new StaticData();
    sd->anchoTablero = 8;
    sd->altoTablero = 8;
    sd->limiteMovimientos = 50;
 
    sd->paredesTablero = new char*[8];
    for(int i = 0; i < 8; i++){
        sd->paredesTablero[i] = new char[8];
        for(int j = 0; j < 8; j++){
            if(i == 0 || i == 7 || j == 0 || j == 7){
                sd->paredesTablero[i][j] = '#';
            } else {
                sd->paredesTablero[i][j] = ' ';
            }
        }
    }
 
    sd->cantidadBloques = 1;
    sd->coloresBloques = new char[1]{'a'};
    sd->anchoBloques = new int[1]{2};
    sd->altoBloques = new int[1]{2};
    sd->geometriaBloques = new uint8_t*[1];
    sd->geometriaBloques[0] = new uint8_t[4]{1, 1, 1, 1};
 
    sd->cantidadSalidas = 1;
    sd->xSalidas = new int[1]{7};
    sd->ySalidas = new int[1]{4};
    sd->coloresSalidas = new char[1]{'a'};
    sd->orientacionSalidas = new char[1]{'V'};
    sd->largoISalidas = new int[1]{2};
    sd->largoFSalidas = new int[1]{2};
    sd->pasosSalidas = new int[1]{0};
 
    sd->cantidadCompuertas = 0;
 
    Bloque** bloques = new Bloque*[1];
    bloques[0] = new Bloque(0, 4, 4, sd);
 
    Salida** salidas = new Salida*[1];
    salidas[0] = new Salida(0, 2, sd);
 
    Compuerta** compuertas = new Compuerta*[0];
 
    Tablero* tablero = new Tablero(sd, bloques, salidas, compuertas);
 
    std::cout << "Estado inicial:" << std::endl;
    tablero->imprimir();
    std::cout << "¿Es solucion? " << (tablero->esSolucion() ? "Si" : "No") << std::endl;
 
    std::cout << "\n--- Prueba 1: chequeos de movimiento ---" << std::endl;
    std::cout << "¿Puede R? " << (tablero->comprobarMovimiento(0, 'R') ? "Si" : "No") << std::endl;
    std::cout << "¿Puede L? " << (tablero->comprobarMovimiento(0, 'L') ? "Si" : "No") << std::endl;
 
    std::cout << "\n--- Prueba 2: clonar antes de mover ---" << std::endl;
    Tablero* clon = tablero->clonar();
    std::cout << "Clon creado. ¿padre del clon = original? "
              << (clon->padre == tablero ? "Si" : "No") << std::endl;
 
    std::cout << "\n--- Prueba 3: aplicar R (1 celda) ---" << std::endl;
    tablero->moverBloque(0, 'R');
    tablero->imprimir();
    std::cout << "contadorMovimientos: " << tablero->contadorMovimientos << std::endl;
 
    std::cout << "\n--- Prueba 4: aplicar R otra vez, debe SALIR automaticamente ---" << std::endl;
    //bloque en x=5, salida en columna 7. Tras mover a x=6, borde derecho=x+2=8, salida xs=7, no adyacente.
    //necesitamos una R más para que el borde quede en 7 (adyDerecha: x+anchoB=xs => x=5). Ya x=5 es adyacente!
    //en realidad después del primer R, x=5, x+2=7=xs: el bloque YA está adyacente a la salida.
    //¿por qué no salió? Porque moverBloque solo chequea salida DESPUES de moverse.
    //La comprobación se hace cada vez que se mueve.
    //Tras el primer R: el bloque está adyacente a la salida. En un A* real eso se detectaría al
    //ejecutar ese primer R. Pero para que salga hay que llamar a intentarSalida o mover otra vez.
    //Probemos una R adicional: quizás no puede porque choca con la salida (que es pared de otro color).
    bool ok = tablero->moverBloque(0, 'R');
    std::cout << "¿Se pudo mover? " << (ok ? "Si" : "No") << std::endl;
    tablero->imprimir();
    std::cout << "cantidadBloques: " << tablero->cantidadBloques << std::endl;
    std::cout << "¿Es solucion? " << (tablero->esSolucion() ? "Si" : "No") << std::endl;
 
    std::cout << "\n--- Prueba 5: el clon sigue intacto ---" << std::endl;
    clon->imprimir();
    std::cout << "clon contadorMovimientos: " << clon->contadorMovimientos << std::endl;

    std::cout << "\n--- Prueba 6: generarVecinos del clon ---" << std::endl;
    int n;
    Tablero** vecinos = clon->generarVecinos(n);
    std::cout << "Vecinos generados: " << n << std::endl;
    for(int i = 0; i < n; i++){
        std::cout << "Vecino " << i << " (movimiento "
                  << (int)vecinos[i]->movimientoOrigen.idBloque
                  << vecinos[i]->movimientoOrigen.direccion << "):" << std::endl;
        vecinos[i]->imprimir();
        std::cout << "  contadorMovimientos: " << vecinos[i]->contadorMovimientos << std::endl;
        std::cout << "  esSolucion? " << (vecinos[i]->esSolucion() ? "Si" : "No") << std::endl;
        std::cout << "  padre apunta al clon? " << (vecinos[i]->padre == clon ? "Si" : "No") << std::endl;
    }
    //limpiar vecinos
    for(int i = 0; i < n; i++) delete vecinos[i];
    delete[] vecinos;

    std::cout << "\n--- Prueba 7: esIgual, hash, heuristica ---" << std::endl;
    //crear un clon del estado inicial del clon (mismo estado, distinto objeto)
    Tablero* gemelo = clon->clonar();
    std::cout << "clon vs gemelo (deben ser iguales):" << std::endl;
    std::cout << "  esIgual? " << (clon->esIgual(gemelo) ? "Si" : "No") << std::endl;
    std::cout << "  hash clon:   " << clon->hash() << std::endl;
    std::cout << "  hash gemelo: " << gemelo->hash() << std::endl;

    //mover el gemelo y comparar
    gemelo->moverBloque(0, 'L');
    std::cout << "\ntras mover gemelo L (deberian ser distintos):" << std::endl;
    std::cout << "  esIgual? " << (clon->esIgual(gemelo) ? "Si" : "No") << std::endl;
    std::cout << "  hash clon:   " << clon->hash() << std::endl;
    std::cout << "  hash gemelo: " << gemelo->hash() << std::endl;

    //volver el gemelo a la posicion original con R: deberian volver a ser iguales
    gemelo->moverBloque(0, 'R');
    std::cout << "\ntras devolver gemelo R (mismo estado espacial, pero tiempo avanzo):" << std::endl;
    std::cout << "  esIgual? " << (clon->esIgual(gemelo) ? "Si" : "No")
              << " (ojo: si hay compuertas/salidas dinámicas pueden diferir)" << std::endl;
    std::cout << "  hash clon:   " << clon->hash() << std::endl;
    std::cout << "  hash gemelo: " << gemelo->hash() << std::endl;

    std::cout << "\nheuristica del clon (bloque en (4,4), salida en x=7): "
              << clon->heuristica() << std::endl;
    //esperado: |4-7| + |4-4| = 3

    delete gemelo;
    delete clon;
    delete tablero;
 
    for(int i = 0; i < 8; i++) delete[] sd->paredesTablero[i];
    delete[] sd->paredesTablero;
    delete[] sd->coloresBloques;
    delete[] sd->anchoBloques;
    delete[] sd->altoBloques;
    delete[] sd->geometriaBloques[0];
    delete[] sd->geometriaBloques;
    delete[] sd->xSalidas;
    delete[] sd->ySalidas;
    delete[] sd->coloresSalidas;
    delete[] sd->orientacionSalidas;
    delete[] sd->largoISalidas;
    delete[] sd->largoFSalidas;
    delete[] sd->pasosSalidas;
    delete sd;
 
    return 0;
}