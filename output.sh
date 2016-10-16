rm *.o
gcc -c ./src/futoshiki.c
gcc -g -o ./build/futoshiki futoshiki.o
clear
[H[2J./build/futoshiki < ./test/test2.in
	FUTOSHIKI 不等式

::: Board 0
NO SOLUTION!

