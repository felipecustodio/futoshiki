rm *.o
# Ultra fast compilation
# gcc -c -O3 -I ./includes ./src/futoshiki.c
# Fast compilation
# gcc -c -O2 -I ./includes ./src/futoshiki.c
# Regular compilation
gcc -c -I ./includes ./src/futoshiki.c
gcc -pg -o ./build/futoshiki futoshiki.o
clear
[H[2J./build/futoshiki < ./test/futoshiki.dat
