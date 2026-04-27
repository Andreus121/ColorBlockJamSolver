all: testBloque testSalida testCompuerta testTablero testMovimiento testParser testMinHeap testHashSet testCamino testAestrella main

StaticData.o: StaticData.h StaticData.cpp
	g++ -c StaticData.cpp

Bloque.o: Bloque.h Bloque.cpp StaticData.o
	g++ -c Bloque.cpp
testBloque: testBloque.cpp Bloque.o
	g++ Bloque.o testBloque.cpp -o testBloque

Salida.o: Salida.h Salida.cpp StaticData.o
	g++ -c Salida.cpp
testSalida: testSalida.cpp Salida.o
	g++ Salida.o testSalida.cpp -o testSalida

Compuerta.o: Compuerta.h Compuerta.cpp StaticData.o
	g++ -c Compuerta.cpp
testCompuerta: testCompuerta.cpp Compuerta.o
	g++ Compuerta.o testCompuerta.cpp -o testCompuerta

Movimiento.o: Movimiento.h Movimiento.cpp
	g++ -c Movimiento.cpp
testMovimiento: testMovimiento.cpp Movimiento.o
	g++ Movimiento.o testMovimiento.cpp -o testMovimiento

Tablero.o: Tablero.h Tablero.cpp StaticData.o Salida.o Compuerta.o Bloque.o Movimiento.o
	g++ -c Tablero.cpp
testTablero: testTablero.cpp Tablero.o
	g++ Bloque.o Salida.o Compuerta.o StaticData.o Tablero.o Movimiento.o testTablero.cpp -o testTablero

Parser.o: Parser.h Parser.cpp StaticData.o Salida.o Compuerta.o Bloque.o Movimiento.o Tablero.o
	g++ -c Parser.cpp
testParser: testParser.cpp Parser.o StaticData.o Salida.o Compuerta.o Bloque.o Movimiento.o Tablero.o
	g++ Bloque.o Salida.o Compuerta.o StaticData.o Parser.o Movimiento.o Tablero.o testParser.cpp -o testParser

MinHeap.o: MinHeap.h MinHeap.cpp Tablero.o
	g++ -c MinHeap.cpp
testMinHeap: testMinHeap.cpp MinHeap.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ MinHeap.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testMinHeap.cpp -o testMinHeap

HashSet.o: HashSet.h HashSet.cpp Tablero.o
	g++ -c HashSet.cpp
testHashSet: testHashSet.cpp HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testHashSet.cpp -o testHashSet

Camino.o: Camino.h Camino.cpp Tablero.o Movimiento.o
	g++ -c Camino.cpp
testCamino: testCamino.cpp Camino.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ Camino.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testCamino.cpp -o testCamino

Aestrella.o: Aestrella.h Aestrella.cpp Tablero.o Camino.o MinHeap.o HashSet.o
	g++ -c Aestrella.cpp
testAestrella: testAestrella.cpp Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o testAestrella.cpp -o testAestrella

main: main.cpp Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o
	g++ Aestrella.o Camino.o MinHeap.o HashSet.o Parser.o Tablero.o Bloque.o Salida.o Compuerta.o StaticData.o Movimiento.o main.cpp -o main

clean:
	rm -f *.o testBloque testSalida testCompuerta testTablero testMovimiento testParser testMinHeap testHashSet testCamino testAestrella