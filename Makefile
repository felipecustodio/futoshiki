CC = gcc
INCLUDES = ./includes
BINARY = ./build/futoshiki

all: clean compile clear run

run:
	# run and test input file
	# $(BINARY) < ./test/futoshiki.dat
	$(BINARY) < ./test/fwtest.in

clean:
	rm *.o

clear:
	clear

compile: futoshiki.o
	$(CC) -pg -o $(BINARY) futoshiki.o

futoshiki.o:
	# Ultra fast compilation
	# $(CC) -c -pg -O3 -I $(INCLUDES) ./src/futoshiki.c
	# Fast compilation
	# $(CC) -c -O2 -I $(INCLUDES) ./src/futoshiki.c
	# Regular compilation
	$(CC) -c -I $(INCLUDES) ./src/futoshiki.c
