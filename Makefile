CC = gcc
INCLUDES = ./includes/
BINARY = ./build/futoshiki

all: clean compile run

run:
	$(BINARY) < ./test/test1.in

clean:
	rm *.o

compile: futoshiki.o
	$(CC) -g -o $(BINARY) futoshiki.o

futoshiki.o:
	$(CC) -c ./src/futoshiki.c
