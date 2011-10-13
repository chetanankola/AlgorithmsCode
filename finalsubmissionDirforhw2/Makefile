# Make file for hw2, Doubly Circular Linked List, Sort and Display
all: hw2

hw2: main.o my570list.o sort.o display.o
	g++ -o hw2 -g main.o my570list.o sort.o display.o

main.o: main.cpp main.hpp
	g++ -g -c -Wall main.cpp -DDEBUG

my570list.o: my570list.cpp my570list.hpp
	g++ -g -c -Wall my570list.cpp -DDEBUG

sort.o: sort.cpp sort.hpp
	g++ -g -Wall -c sort.cpp -DDEBUG

display.o: display.cpp display.hpp
	g++ -g -Wall -c display.cpp -DDEBUG

clean: 
	rm -f *.o hw2
