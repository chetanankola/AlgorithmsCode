# Make file for hw4, Maze generation and Maze solving
all: hw5

hw5: main.o util.o my570list.o sudokugen.o sudokusolve.o sudokucount.o
	g++ -o hw5 -g main.o util.o my570list.o sudokugen.o sudokusolve.o sudokucount.o

main.o: main.cpp main.hpp
	g++ -g -c -Wall main.cpp -DDEBUG

util.o: util.cpp util.hpp
	g++ -g -c -Wall util.cpp -DDEBUG

my570list.o: my570list.cpp my570list.hpp
	g++ -g -c -Wall my570list.cpp -DDEBUG

sudokugen.o: sudokugen.cpp sudokugen.hpp
	g++ -g -c -Wall sudokugen.cpp -DDEBUG

sudokusolve.o: sudokusolve.cpp sudokusolve.hpp
	g++ -g -c -Wall sudokusolve.cpp -DDEBUG	

sudokucount.o: sudokucount.cpp sudokucount.hpp
	g++ -g -c -Wall sudokucount.cpp -DDEBUG	
	
clean: 
	rm -f *.o hw5
