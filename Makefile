CC = gcc
INCLUDES = ./includes
BINARY = ./build/futoshiki

all: clean compile clear run

run:
	# $(BINARY) < ./test/fwtest.in
	$(BINARY) < ./test/futoshiki.dat

clean:
	rm *.o

clear:
	clear

compile: futoshiki.o
	$(CC) -pg -o $(BINARY) futoshiki.o

futoshiki.o:
	$(CC) -c -I $(INCLUDES) ./src/futoshiki.c
