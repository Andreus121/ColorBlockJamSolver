#include <iostream>
#include "Tablero.h"

Tablero::Tablero(StaticData* staticData,
        Bloque** bloques,
        Salida** salidas,
        Compuerta** compuertas){
    this->staticData = staticData;
    this->bloques = bloques;
    this->salidas = salidas;
    this->compuertas = compuertas;

    //el estado inicial parte con todos los bloques en juego
    this->cantidadBloques = staticData->cantidadBloques;
    this->contadorMovimientos = 0;

    //estado inicial no tiene padre ni movimiento previo
    this->padre = nullptr;
    this->movimientoOrigen = Movimiento();

    //reservar cuadricula vacía; se llena con reconstruirCuadricula()
    int alto = staticData->altoTablero;
    int ancho = staticData->anchoTablero;
    this->cuadricula = new char*[alto];
    for(int i = 0; i < alto; i++){
        this->cuadricula[i] = new char[ancho];
    }

    reconstruirCuadricula();
}

Tablero::~Tablero(){
    //liberar cuadricula
    //(puede haber sido liberada por liberarCuadricula() para ahorrar memoria durante A*
    if(this->cuadricula != nullptr){
        int alto = staticData->altoTablero;
        for(int i = 0; i < alto; i++){
            delete[] this->cuadricula[i];
        }
        delete[] this->cuadricula;
        this->cuadricula = nullptr;
    }

    //liberar bloques vivos
    for(int i = 0; i < this->cantidadBloques; i++){
        delete this->bloques[i];
    }
    delete[] this->bloques;

    //liberar salidas
    for(int i = 0; i < this->staticData->cantidadSalidas; i++){
        delete this->salidas[i];
    }
    delete[] this->salidas;

    //liberar compuertas
    for(int i = 0; i < this->staticData->cantidadCompuertas; i++){
        delete this->compuertas[i];
    }
    delete[] this->compuertas;

    //staticData NO se libera aquí: lo comparten todos los tableros
    //padre NO se libera aquí: la gestión de nodos la hace A*
}

//rellena todo el tablero
void Tablero::reconstruirCuadricula(){
    int alto = this->staticData->altoTablero;
    int ancho = this->staticData->anchoTablero;

    //si la cuadricula fue liberada (por ahorro de memoria), la reservamos de nuevo
    if(this->cuadricula == nullptr){
        this->cuadricula = new char*[alto];
        for(int i = 0; i < alto; i++){
            this->cuadricula[i] = new char[ancho];
        }
    }

    //copiar paredes base desde staticData
    //'#' pared, ' ' vacío
    for(int i = 0; i < alto; i++){
        for(int j = 0; j < ancho; j++){
            this->cuadricula[i][j] = this->staticData->paredesTablero[i][j];
        }
    }

    //pintar salidas según su largo actual
    //orientación 'H' (horizontal): ocupa celdas en x, y es fijo
    //orientación 'V' (vertical): ocupa celdas en y, x es fijo
    for(int s = 0; s < this->staticData->cantidadSalidas; s++){
        Salida* salida = this->salidas[s];
        int x0 = this->staticData->xSalidas[s];
        int y0 = this->staticData->ySalidas[s];
        char color = this->staticData->coloresSalidas[s];
        char orient = this->staticData->orientacionSalidas[s];
        int largoActual = salida->la;

        for(int k = 0; k < largoActual; k++){
            int fila, col;
            if(orient == 'H'){
                fila = y0;
                col = x0 + k;
            } else {
                fila = y0 + k;
                col = x0;
            }
            //seguridad solo pintar dentro del tablero
            if(fila >= 0 && fila < alto && col >= 0 && col < ancho){
                this->cuadricula[fila][col] = color;
            }
        }
    }

    //pintar compuertas con su color actual
    //pintar compuertas con su color actual, considerando largo y orientacion
    for(int c = 0; c < this->staticData->cantidadCompuertas; c++){
        int x0 = this->staticData->xCompuertas[c];
        int y0 = this->staticData->yCompuertas[c];
        int largo = this->staticData->largoCompuertas[c];
        char orient = this->staticData->orientacionCompuertas[c];
        char color = this->compuertas[c]->coloractual;

        for(int k = 0; k < largo; k++){
            int fila, col;
            if(orient == 'H'){ fila = y0; col = x0 + k; }
            else { fila = y0 + k; col = x0; }
            this->cuadricula[fila][col] = color;
        }
    }

    //pintar bloques según su geometría
    //geometriaBloques[idBloque] es un arreglo plano de ancho*alto booleanos
    //fila i, col j del bloque => index = i*anchoBloque + j
    for(int b = 0; b < this->cantidadBloques; b++){
        Bloque* bloque = this->bloques[b];
        int id = bloque->id;
        int anchoB = this->staticData->anchoBloques[id];
        int altoB = this->staticData->altoBloques[id];
        char colorB = this->staticData->coloresBloques[id];
        uint8_t* geom = this->staticData->geometriaBloques[id];

        for(int i = 0; i < altoB; i++){
            for(int j = 0; j < anchoB; j++){
                if(geom[i * anchoB + j] == 1){
                    int fila = bloque->y + i;
                    int col = bloque->x + j;
                    this->cuadricula[fila][col] = colorB;
                }
            }
        }
    }
}

//muestra el tablero en pantalla
void Tablero::imprimir(){
    int alto = this->staticData->altoTablero;
    int ancho = this->staticData->anchoTablero;
    for(int i = 0; i < alto; i++){
        for(int j = 0; j < ancho; j++){
            std::cout << this->cuadricula[i][j];
        }
        std::cout << std::endl;
    }
}

//los metodos usados para movimientos y A*

//comprobar si el juego termino
bool Tablero::esSolucion(){
    return this->cantidadBloques == 0;
}

//constructor usado por A* para clonar
Tablero::Tablero(StaticData* staticData,
        int cantidadBloques,
        Bloque** bloques,
        Salida** salidas,
        Compuerta** compuertas,
        int contadorMovimientos,
        Tablero* padre){
    this->staticData = staticData;
    this->cantidadBloques = cantidadBloques;
    this->bloques = bloques;
    this->salidas = salidas;
    this->compuertas = compuertas;
    this->contadorMovimientos = contadorMovimientos;
    this->padre = padre;
    this->movimientoOrigen = Movimiento();

    int alto = staticData->altoTablero;
    int ancho = staticData->anchoTablero;
    this->cuadricula = new char*[alto];
    for(int i = 0; i < alto; i++){
        this->cuadricula[i] = new char[ancho];
    }
    reconstruirCuadricula();
}

//clonar el tablero actual
Tablero* Tablero::clonar(){
    //copiar bloques vivos
    Bloque** nuevosBloques = new Bloque*[this->cantidadBloques];
    for(int i = 0; i < this->cantidadBloques; i++){
        Bloque* orig = this->bloques[i];//crea el arreglo
        nuevosBloques[i] = new Bloque(orig->id, orig->x, orig->y, this->staticData);//agrega cada bloque
    }

    //copiar salidas (todas, aunque no haya bloques adentro siguen existiendo)
    int numSalidas = this->staticData->cantidadSalidas;
    Salida** nuevasSalidas = new Salida*[numSalidas];
    for(int i = 0; i < numSalidas; i++){
        Salida* orig = this->salidas[i];
        Salida* copia = new Salida(orig->id, orig->la, this->staticData);
        copia->contadorpasos = orig->contadorpasos;
        copia->direccion = orig->direccion;
        nuevasSalidas[i] = copia;
    }

    //copiar compuertas
    int numCompuertas = this->staticData->cantidadCompuertas;
    Compuerta** nuevasCompuertas = new Compuerta*[numCompuertas];
    for(int i = 0; i < numCompuertas; i++){
        Compuerta* orig = this->compuertas[i];
        Compuerta* copia = new Compuerta(orig->id, orig->coloractual, this->staticData);
        copia->contadorpasos = orig->contadorpasos;
        nuevasCompuertas[i] = copia;
    }

    //crear nuevo tablero con this como padre
    Tablero* clon = new Tablero(
        this->staticData,
        this->cantidadBloques,
        nuevosBloques,
        nuevasSalidas,
        nuevasCompuertas,
        this->contadorMovimientos,
        this);
    return clon;
}

//retorna el indice del bloque con ese id o -1 si no existe
int Tablero::buscarBloque(uint8_t idBloque){
    for(int i = 0; i < this->cantidadBloques; i++){
        if((uint8_t)this->bloques[i]->id == idBloque){
            return i;
        }
    }
    return -1;
}

bool Tablero::comprobarMovimiento(uint8_t idBloque, char direccion){
    //buscar el indice del bloque
    int idx = buscarBloque(idBloque);
    //si no existe no se valida el movimiento
    if(idx == -1) return false;

    //obtener el puntero al bloque
    Bloque* bloque = this->bloques[idx];
    int id = bloque->id;
    int anchoB = this->staticData->anchoBloques[id];
    int altoB = this->staticData->altoBloques[id];
    uint8_t* geom = this->staticData->geometriaBloques[id];

    //desplazar 1 celda en la dirección dada
    int dx = 0, dy = 0;
    if(direccion == 'U') dy = -1;
    else if(direccion == 'D') dy = 1;
    else if(direccion == 'L') dx = -1;
    else if(direccion == 'R') dx = 1;
    else return false;

    //posición destino del bloque
    int nuevoX = bloque->x + dx;
    int nuevoY = bloque->y + dy;

    //recorrer cada celda ocupada por la geometría en la NUEVA posición
    for(int i = 0; i < altoB; i++){
        for(int j = 0; j < anchoB; j++){
            if(geom[i * anchoB + j] != 1){
                continue; // si la geometria no ocupa la celda, no chequeamos colisiones ahí
            }
            //calcular las coordenadas absolutas de la celda a validar
            int fila = nuevoY + i;
            int col = nuevoX + j;

            char contenido = this->cuadricula[fila][col];
            if(contenido == ' '){
                continue; // vacío, ok
            }

            //la celda tiene algo, verificar si es parte del mismo bloque en su pos actual
            int localI = fila - bloque->y;//indice dentro de geometria
            int localJ = col - bloque->x;//indice dentro de geometria
            bool esMismoBloque = false;
            //comprobar limites de geometria
            if(localI >= 0 && localI < altoB && localJ >= 0 && localJ < anchoB){
                //verificar si es de la misma geometria
                if(geom[localI * anchoB + localJ] == 1){ 
                    esMismoBloque = true;
                }
            }
            if(!esMismoBloque) {
                return false;
            } //choca con algo ajeno
        }
    }
    return true; //no choca con nada
}


bool Tablero::intentarSalida(uint8_t idBloque){
    //buscar el indice del bloque
    int idx = buscarBloque(idBloque);
    //si no existe no se puede salir
    if(idx == -1) return false;

    //guardar los datos del bloque
    Bloque* bloque = this->bloques[idx];
    int id = bloque->id;
    int anchoB = this->staticData->anchoBloques[id];
    int altoB = this->staticData->altoBloques[id];
    char colorB = this->staticData->coloresBloques[id];

    //ver todas las salidas
    for(int s = 0; s < this->staticData->cantidadSalidas; s++){
        //color debe coincidir
        if(this->staticData->coloresSalidas[s] != colorB) continue;

        //guardar datos de la salida
        int xs = this->staticData->xSalidas[s];
        int ys = this->staticData->ySalidas[s];
        char orient = this->staticData->orientacionSalidas[s];
        int largoActual = this->salidas[s]->la;

        //casos segun orientacion y lado del tablero
        //salida vertical (V) en la pared derecha o izquierda
        if(orient == 'V'){
            //la salida ocupa filas desde ys hasta ys+largoActual-1, en columna xs
            //bloque a la derecha saliendo: borde derecho del bloque adyacente a xs
            //bloque a la izquierda saliendo: borde izquierdo del bloque adyacente a xs
            bool adyDerecha = (bloque->x + anchoB == xs);
            bool adyIzquierda = (bloque->x - 1 == xs);
            if(!adyDerecha && !adyIzquierda){
                continue;
            }
            //el alto del bloque debe ser <= largoActual
            if(altoB > largoActual) {
                continue;
            }
            //las filas del bloque (y..y+altoB-1) deben estar DENTRO del rango de la salida
            if(bloque->y < ys){
                continue;
            }
            if(bloque->y + altoB > ys + largoActual){
                continue;
            }

            //todo ok: el bloque sale
            //eliminar el bloque del arreglo
            delete this->bloques[idx];
            for(int k = idx; k < this->cantidadBloques - 1; k++){
                this->bloques[k] = this->bloques[k + 1];
            }
            this->cantidadBloques--;
            return true;
        }
        //salida horizontal (H) en la pared arriba o abajo
        else {
            //la salida ocupa columnas xs..xs+largoActual-1, en fila ys
            bool adyAbajo = (bloque->y + altoB == ys);
            bool adyArriba = (bloque->y - 1 == ys);
            if(!adyAbajo && !adyArriba){
                continue;
            }

            //si el bloque no cabe, sigue
            if(anchoB > largoActual) {
                continue;
            }

            //comprobar que el bloque este entre xs y xs+largoActual-1
            if(bloque->x < xs) {
                continue;
            }
            if(bloque->x + anchoB > xs + largoActual) {
                continue;
            }

            //todo ok: el bloque sale
            //eliminar el bloque del arreglo
            delete this->bloques[idx];
            for(int k = idx; k < this->cantidadBloques - 1; k++){
                this->bloques[k] = this->bloques[k + 1];
            }
            this->cantidadBloques--;
            return true;
        }
    }
    return false;
}

bool Tablero::intentarCompuerta(uint8_t idBloque){
    int idx = buscarBloque(idBloque);
    if(idx == -1) return false;

    Bloque* bloque = this->bloques[idx];
    int id = bloque->id;
    int anchoB = this->staticData->anchoBloques[id];
    int altoB = this->staticData->altoBloques[id];
    char colorB = this->staticData->coloresBloques[id];
    uint8_t* geom = this->staticData->geometriaBloques[id];

    int alto = this->staticData->altoTablero;
    int ancho = this->staticData->anchoTablero;

    for(int c = 0; c < this->staticData->cantidadCompuertas; c++){
        if(this->compuertas[c]->coloractual != colorB) continue;

        int xc = this->staticData->xCompuertas[c];
        int yc = this->staticData->yCompuertas[c];
        int largo = this->staticData->largoCompuertas[c];
        char orient = this->staticData->orientacionCompuertas[c];

        int dx = 0, dy = 0;
        bool adyacente = false;

        if(orient == 'V'){
            //compuerta vertical: ocupa filas yc..yc+largo-1 en columna xc
            //bloque adyacente por la derecha (sale a la izquierda de la compuerta)
            if(bloque->x + anchoB == xc
               && bloque->y >= yc && bloque->y + altoB <= yc + largo
               && altoB <= largo){
                dx = anchoB + 1; adyacente = true;
            }
            //bloque adyacente por la izquierda
            else if(bloque->x - 1 == xc
                    && bloque->y >= yc && bloque->y + altoB <= yc + largo
                    && altoB <= largo){
                dx = -(anchoB + 1); adyacente = true;
            }
        } else {
            //compuerta horizontal: ocupa columnas xc..xc+largo-1 en fila yc
            //bloque adyacente por abajo
            if(bloque->y + altoB == yc
               && bloque->x >= xc && bloque->x + anchoB <= xc + largo
               && anchoB <= largo){
                dy = altoB + 1; adyacente = true;
            }
            //bloque adyacente por arriba
            else if(bloque->y - 1 == yc
                    && bloque->x >= xc && bloque->x + anchoB <= xc + largo
                    && anchoB <= largo){
                dy = -(altoB + 1); adyacente = true;
            }
        }

        if(!adyacente) continue;

        int nuevoX = bloque->x + dx;
        int nuevoY = bloque->y + dy;

        bool hayEspacio = true;
        for(int i = 0; i < altoB && hayEspacio; i++){
            for(int j = 0; j < anchoB && hayEspacio; j++){
                if(geom[i * anchoB + j] != 1) continue;
                int fila = nuevoY + i;
                int col = nuevoX + j;
                if(fila < 0 || fila >= alto || col < 0 || col >= ancho){
                    hayEspacio = false; break;
                }
                if(this->cuadricula[fila][col] != ' ') hayEspacio = false;
            }
        }
        if(!hayEspacio) continue;

        bloque->x = nuevoX;
        bloque->y = nuevoY;
        return true;
    }
    return false;
}

bool Tablero::moverBloque(uint8_t idBloque, char direccion){
    //verifica si se puede hacer el movimiento
    if(!comprobarMovimiento(idBloque, direccion)){
         return false;
    }
    //buscar el indice del bloque a mover
    int idx = buscarBloque(idBloque);
    //guardar el puntero del bloque a mover
    Bloque* bloque = this->bloques[idx];

    //aplicar el desplazamiento de 1 celda
    if(direccion == 'U') bloque->y -= 1;
    else if(direccion == 'D') bloque->y += 1;
    else if(direccion == 'L') bloque->x -= 1;
    else if(direccion == 'R') bloque->x += 1;

    
    //actualizar compuertas y salidas según su contador de pasos
    for(int c = 0; c < this->staticData->cantidadCompuertas; c++){
        this->compuertas[c]->actualizarCompuerta(
            this->staticData->colorICompuertas[c],
            this->staticData->colorFCompuertas[c],
            this->staticData->pasosCompuertas[c]);
    }
    for(int s = 0; s < this->staticData->cantidadSalidas; s++){
        this->salidas[s]->actualizarSalida(
            this->staticData->largoISalidas[s],
            this->staticData->largoFSalidas[s],
            this->staticData->pasosSalidas[s]);
    }
    //avanzar el tiempo: 1 paso por celda deslizada
    this->contadorMovimientos += 1;

    //registrar el movimiento
    this->movimientoOrigen = Movimiento(idBloque, direccion, 1);
    
    //reconstruir cuadricula con la nueva posición + tiempo avanzado
    reconstruirCuadricula();

    //ahora chequear eventos post-movimiento (sin costo adicional)
    if(intentarSalida(idBloque)){
        reconstruirCuadricula(); //se eliminó el bloque
        return true;
    }
    if(intentarCompuerta(idBloque)){
        reconstruirCuadricula(); //cambió la posición
        return true;
    }

    return true;
}

//retornar la suma de las distancias de cada bloque con su salida
int Tablero::contarMovimientosNecesarios(){
    int total = 0;
    for(int b = 0; b < this->cantidadBloques; b++){
        //obtener el puntero del bloque y sus datos
        Bloque* bloque = this->bloques[b];
        int id = bloque->id;
        char color = this->staticData->coloresBloques[id];
        int anchoB = this->staticData->anchoBloques[id];
        int altoB = this->staticData->altoBloques[id];

        //buscar la salida del mismo color más cercana
        int mejorDist = -1;
        for(int s = 0; s < this->staticData->cantidadSalidas; s++){
            //verificar el mismo color
            if(this->staticData->coloresSalidas[s] != color){
                continue;
            }

            //usamos como referencia el centro del bloque y la posición de la salida
            int bx = bloque->x;
            int by = bloque->y;
            int sx = this->staticData->xSalidas[s];
            int sy = this->staticData->ySalidas[s];

            //distancia desde el BORDE del bloque mas cercano a la salida.
            //Si la salida esta a la derecha del bloque (sx >= x+anchoB),
            //el borde derecho del bloque tiene que recorrer sx-(x+anchoB) celdas.
            //Si esta a la izquierda (sx < x), el borde izquierdo recorre x-sx.
            //Si la salida esta dentro del rango del bloque, distancia 0 en X.
            int dx;
            if(sx >= bloque->x + anchoB) dx = sx - (bloque->x + anchoB);
            else if(sx < bloque->x) dx = bloque->x - sx;
            else dx = 0;

            int dy;
            if(sy >= bloque->y + altoB) dy = sy - (bloque->y + altoB);
            else if(sy < bloque->y) dy = bloque->y - sy;
            else dy = 0;

            int dist = dx + dy;
            //guardar solo la menor distancia a una salida del mismo color
            if(mejorDist == -1 || dist < mejorDist){
                mejorDist = dist;
            }
        }

        //si no hay salida del color, el tablero no tiene solución
        //por lo que se devuelve una heurestica alta
        //para procesar la decision en otra funcion
        if(mejorDist == -1){
            mejorDist = 1000;
        }
        total += mejorDist;
    }
    return total;
}

//genera los tableros vecinos aplicando movimientos legales de cada bloque en cada dirección
Tablero** Tablero::generarVecinos(int& cantidadVecinos){
    //capacidad máxima: cada bloque puede ir en 4 direcciones
    int capacidad = this->cantidadBloques * 4;
    if(capacidad == 0){
        cantidadVecinos = 0;
        return nullptr;
    }
    //listado de tableros posibles
    Tablero** vecinos = new Tablero*[capacidad];
    int n = 0;

    char direcciones[4] = {'U', 'D', 'L', 'R'};

    //por cada bloque vivo
    for(int b = 0; b < this->cantidadBloques; b++){
        uint8_t idBloque = this->bloques[b]->id;
        for(int d = 0; d < 4; d++){
            //chequear si el movimiento es legal en este tablero (sin clonar)
            if(!this->comprobarMovimiento(idBloque, direcciones[d])) continue;

            //clonar y aplicar el movimiento al clon
            Tablero* hijo = this->clonar();
            hijo->moverBloque(idBloque, direcciones[d]);
            //moverBloque ya setea hijo->movimientoOrigen y hijo->padre ya es 'this'
            vecinos[n] = hijo;
            n++;
        }
    }

    cantidadVecinos = n;
    return vecinos;
}

//Los tableros son iguales si tienen los mismos bloques vivos en las mismas
//posiciones, las mismas compuertas con los mismos colores actuales, y las mismas
//salidas con los mismos largos actuales.
bool Tablero::esIgual(Tablero* otro){
    // cantidad de bloques
    if(this->cantidadBloques != otro->cantidadBloques){
        return false;
    }

    //Comparar bloques mediante id ya que indice puede ser distinto
    for(int i = 0; i < this->cantidadBloques; i++){
        Bloque* b1 = this->bloques[i];
        //buscar el bloque del mismo id en el otro tablero
        bool encontrado = false;
        for(int j = 0; j < otro->cantidadBloques; j++){
            Bloque* b2 = otro->bloques[j];
            //verificar si tienen el mismo id
            if(b1->id == b2->id){
                //verificar si tienen las mismas coordenadas
                if(b1->x != b2->x || b1->y != b2->y){
                    return false;
                }
                //encontró el bloque, puede seguir con el siguiente
                encontrado = true;
                break;
            }
        }
        //si no encuentra el bloque, los tableros no son iguales
        if(!encontrado){
            return false;
        }
    }

    //compuertas: mismo orden en la lista de cada tablero
    int nc = this->staticData->cantidadCompuertas;
    for(int i = 0; i < nc; i++){
        //verificar el color y pasos de cada uno
        if(this->compuertas[i]->coloractual != otro->compuertas[i]->coloractual){
            return false;
        }
        if(this->compuertas[i]->contadorpasos != otro->compuertas[i]->contadorpasos){
            return false;
        }
    }

    //salidas:mismo orden en la lista de cada tablero
    int ns = this->staticData->cantidadSalidas;
    for(int i = 0; i < ns; i++){
        //comprobar largo actual, pasos y direccion
        if(this->salidas[i]->la != otro->salidas[i]->la){
            return false;
        }
        if(this->salidas[i]->contadorpasos != otro->salidas[i]->contadorpasos){
            return false;
        }
        if(this->salidas[i]->direccion != otro->salidas[i]->direccion){
            return false;
        }
    }
    //verificó todo, son iguales
    return true;
}

//Hash simple: combina posiciones de bloques, colores de compuertas, largos de salidas.
//Se multiplica por un primo grande en cada iteracion para dispersar.
unsigned long Tablero::hash(){
    unsigned long h = 0;
    const unsigned long PRIMO = 1000003UL; //primo para mezclar bits

    //incorporar posiciones de bloques (ordenados por id para consistencia)
    //si los bloques están en orden de id, podemos iterar directo; si no,
    //para mayor robustez iteramos por cantidadBloquesTotal y buscamos cada id
    int nb = this->staticData->cantidadBloques;
    for(int id = 0; id < nb; id++){
        //buscar si el bloque 'id' sigue vivo
        int x = -1; //-1 indica que salió
        int y = -1; //-1 indica que salió
        for(int i = 0; i < this->cantidadBloques; i++){
            if((int)this->bloques[i]->id == id){
                x = this->bloques[i]->x;
                y = this->bloques[i]->y;
                break;
            }
        }
        //sumar resultados al hash
        h = h * PRIMO + (unsigned long)(x + 1000);
        h = h * PRIMO + (unsigned long)(y + 1000);
    }

    //sumar los colores de las cmpuertas y sus pasos
    for(int i = 0; i < this->staticData->cantidadCompuertas; i++){
        h = h * PRIMO + (unsigned long)this->compuertas[i]->coloractual;
        h = h * PRIMO + (unsigned long)this->compuertas[i]->contadorpasos;
    }

    //sumar largo de las salidas y sus pasos
    for(int i = 0; i < this->staticData->cantidadSalidas; i++){
        h = h * PRIMO + (unsigned long)this->salidas[i]->la;
        h = h * PRIMO + (unsigned long)this->salidas[i]->contadorpasos;
    }

    return h;
}

//Heurística: suma de distancias Manhattan de cada bloque vivo a la salida
//mas cercana de su mismo color. Esto subestima la distancia real cuando
//hay paredes/compuertas en el camino, pero es admisible: A* encuentra la
//solucion optima si existe.
int Tablero::heuristica(){
    int total = 0;
    for(int b = 0; b < this->cantidadBloques; b++){
        //obtener el puntero del bloque y sus datos
        Bloque* bloque = this->bloques[b];
        int id = bloque->id;
        char color = this->staticData->coloresBloques[id];

        //buscar la salida del mismo color más cercana
        int mejorDist = -1;
        for(int s = 0; s < this->staticData->cantidadSalidas; s++){
            //verificar el mismo color
            if(this->staticData->coloresSalidas[s] != color){
                continue;
            }

            //usamos como referencia el centro del bloque y la posición de la salida
            int bx = bloque->x;
            int by = bloque->y;
            int sx = this->staticData->xSalidas[s];
            int sy = this->staticData->ySalidas[s];

            //calcular distancia mediante distancia Manhattan: |dx| + |dy|
            int dx = bx - sx; 
            if(dx < 0){
                dx = -dx;
            }
            int dy = by - sy; 
            if(dy < 0){
                dy = -dy;
            }
            int dist = dx + dy;

            //guardar solo la menor distancia a una salida del mismo color
            if(mejorDist == -1 || dist < mejorDist){
                mejorDist = dist;
            }
        }

        //si no hay salida del color, el tablero no tiene solución
        //por lo que se devuelve una heurestica alta
        //para procesar la decision en otra funcion
        if(mejorDist == -1){
            mejorDist = 1000;
        }
        total += mejorDist;
    }
    return total;
}

//Libera el arreglo de cuadricula para ahorrar memoria.
//Usado por A* al mover tableros al ClosedSet: alli solo nos interesa
//comparar estados por hash/esIgual y seguir punteros padre, no ver la visual.
//Si alguien despues llama imprimir() o moverBloque() sobre este tablero,
//reconstruirCuadricula() la regenerara automaticamente.
void Tablero::liberarCuadricula(){
    if(this->cuadricula == nullptr) return;
    int alto = this->staticData->altoTablero;
    for(int i = 0; i < alto; i++){
        delete[] this->cuadricula[i];
    }
    delete[] this->cuadricula;
    this->cuadricula = nullptr;
}