# Make file for hw2, Doubly Circular Linked List, Sort and Display
all: hw3

hw3: main.o util.o my570BST.o BST.o AVL.o
	g++ -o hw3 -g main.o util.o my570BST.o BST.o AVL.o

main.o: main.cpp main.hpp
	g++ -g -c -Wall main.cpp -DDEBUG

util.o: util.cpp util.hpp
	g++ -g -c -Wall util.cpp -DDEBUG

my570BST.o: my570BST.cpp my570BST.hpp
	g++ -g -c -Wall my570BST.cpp -DDEBUG

BST.o: BST.cpp BST.hpp
	g++ -g -c -Wall BST.cpp -DDEBUG

AVL.o: AVL.cpp AVL.hpp
	g++ -g -c -Wall AVL.cpp -DDEBUG
	
clean: 
	rm -f *.o hw3
