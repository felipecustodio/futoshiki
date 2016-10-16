CC = gcc
INCLUDES = ./includes
BINARY = ./build/futoshiki

all: clean compile clear run

run:
	$(BINARY) < ./test/test2.in

clean:
	rm *.o

clear:
	clear

compile: futoshiki.o
	$(CC) -g -o $(BINARY) futoshiki.o

futoshiki.o:
	$(CC) -c -I $(INCLUDES) ./src/futoshiki.c
