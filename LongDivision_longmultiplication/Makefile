# Make file for hw1, Long Division and Long Multiplication
all: hw1
hw1: main.o mul.o div.o test.o
	gcc -o hw1 -g main.o mul.o div.o test.o
main.o: src/main.c
	gcc -g -Wall -c src/main.c
mul.o: src/mul.c
	gcc -g -Wall -c src/mul.c
div.o: src/div.c
	gcc -g -Wall -c src/div.c
test.o: test/test.c
	gcc -g -Wall -c test/test.c
clean: 
	rm -f *.o hw1
	
