#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "board.h"
#include "futoshiki.h"


int main(int argc, char const *argv[]) {

    // benchmarking
    clock_t start_t, end_t, total_t;
    float delta_t = 0.0;

    printf("\tFUTOSHIKI 不等式\n\n");
    int i;
    int calls;
    int n = 0;
    scanf("%d", &n);
    BOARD** boards = readBoards(n);
    // start timer
    start_t = clock();
    for (i = 0; i < n; i++) {
        calls = 0;
        printf("::: Board %d\n", i+1);
        if (futoshiki_simple(&boards[i], 0, 0, &calls)) {
            printf(":: %d calls\n", calls);
            printBoard(boards[i]);
        } else {
            if (calls >= OVERFLOW) {
                printf("::: ｏｖｅｒｆｌｏｗ\n");
                printf(":: %d calls.\n", calls);
            }
            printf("Can't solve this board!\n");
        }
        printf("\n");
    }
    // end timer
    end_t = clock();
    // human readable time
    delta_t = ((float)(end_t - start_t) / 1000000.0 ) * CLOCKS_PER_SEC;
    printf("::: Total time %lf\n", delta_t);

    // // DESTROY MEMORY
    // int j, k;
    // for (i = 0; i < n; i++) {
    //     int aux;
    //     for (i = 0; i < boards[i]->n; i++) {
    //         for (j = 0; j < boards[i]->n; j++) {
    //             aux = boards[i]->matrix[i][j].r;
    //             for(k = 0; k < aux; k++) {
    //                 free(boards[i]->matrix[i][j].restrictions[k]);
    //                 boards[i]->matrix[i][j].restrictions[k] = NULL;
    //             }
    //             free(boards[i]->matrix[i][j].restrictions);
    //             boards[i]->matrix[i][j].restrictions = NULL;
    //         }
    //         free(boards[i]->matrix[i]);
    //         boards[i]->matrix[i] = NULL;
    //     }
    //     free(boards[i]->matrix);
    //     boards[i]->matrix = NULL;
    //     free(boards[i]);
    //     boards[i] = NULL;
    // }
    // free(boards);

    printf("PROGRAM FINISHED.\n");
    return 0;
}

