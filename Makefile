all: testBloque testSalida testCompuerta

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

clean:
	rm -f *.o testBloque testSalida testCompuerta