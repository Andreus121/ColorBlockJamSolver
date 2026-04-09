all: testPieza testTablero

Pieza.o: Pieza.cpp Pieza.h
	g++ -c Pieza.cpp
testPieza: testPieza.cpp Pieza.h
	g++ -o testPieza testPieza.cpp

Tablero.o: Tablero.cpp Tablero.h Pieza.h
	g++ -c Tablero.cpp
testTablero: testTablero.cpp Tablero.o Pieza.o Tablero.h Pieza.h
	g++ Tablero.o Pieza.o testTablero.cpp -o testTablero 

clean:
	rm -f *.o testPieza testTablero