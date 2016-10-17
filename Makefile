CC = gcc
INCLUDES = ./includes
BINARY = ./build/futoshiki

all: clean compile clear run

run:
	$(BINARY) < ./test/futoshiki.dat

clean:
	rm *.o

clear:
	clear

compile: main.o board.o futoshiki.o
	$(CC) -pg -o $(BINARY) main.o board.o futoshiki.o 

board.o:
	$(CC) -c -I $(INCLUDES) ./src/board.c

futoshiki.o:
	$(CC) -c -I $(INCLUDES) ./src/futoshiki.c

main.o:
	$(CC) -c -I $(INCLUDES) ./src/main.c

