# Make file for hw1, Long Division and Long Multiplication
all: hw1
hw1: main.o mul.o div.o test.o
	gcc -o hw1 -g main.o mul.o div.o test.o
main.o: src/main.c
	gcc -g -Wall -c src/main.c -DDEBUG
mul.o: src/mul.c
	gcc -g -Wall -c src/mul.c -DDEBUG
div.o: src/div.c
	gcc -g -Wall -c src/div.c -DDEBUG
test.o: test/test.c
	gcc -g -Wall -c test/test.c -DDEBUG
clean: 
	rm -f *.o hw1
	
