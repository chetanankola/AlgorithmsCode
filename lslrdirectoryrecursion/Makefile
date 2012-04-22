# Make file for hw4, Maze generation and Maze solving
all: hw4

hw4: main.o util.o my570list.o mazegen.o mazesolve.o
	g++ -o hw4 -g main.o util.o my570list.o mazegen.o mazesolve.o

main.o: main.cpp main.hpp
	g++ -g -c -Wall main.cpp -DDEBUG

util.o: util.cpp util.hpp
	g++ -g -c -Wall util.cpp -DDEBUG

my570list.o: my570list.cpp my570list.hpp
	g++ -g -c -Wall my570list.cpp -DDEBUG

mazegen.o: mazegen.cpp mazegen.hpp
	g++ -g -c -Wall mazegen.cpp -DDEBUG

mazesolve.o: mazesolve.cpp mazesolve.hpp
	g++ -g -c -Wall mazesolve.cpp -DDEBUG	
	
clean: 
	rm -f *.o hw4
