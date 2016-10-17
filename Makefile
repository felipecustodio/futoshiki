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

compile: futoshiki.o
	$(CC) -pg -o $(BINARY) futoshiki.o

futoshiki.o:
	# Ultra fast compilation
	$(CC) -c -O3 -I $(INCLUDES) ./src/futoshiki.c
	# Fast compilation
	# $(CC) -c -O2 -I $(INCLUDES) ./src/futoshiki.c
	# Regular compilation
	# $(CC) -c -I $(INCLUDES) ./src/futoshiki.c
