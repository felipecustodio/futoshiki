#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "board.h"

// BACKTRACKING - SIMPLE, NO HEURISTICS
bool futoshiki_simple(BOARD** b, int x, int y, int* calls) {
    int i;
    // check if recursive calls reached overflow
    if (*calls >= OVERFLOW) {
        return FALSE;
    }
    // check if has reached end of board
    if (x >= (*b)->n || y >= (*b)->n) {
        return TRUE;
    }
    // try possible values
    for (i = 1; i <= (*b)->n; i++) {
        // check if cell is occupied
        if ((*b)->matrix[x][y].value == 0) {
            // don't change pre-filled values
            if (!((*b)->matrix[x][y].filled)) {
                (*b)->matrix[x][y].value = i;
                (*calls)++;
            }
        }
        // check if value isn't already placed in conflicting cells
        if (isValid(*b, x, y)) {
            // checks if reached last column
            if (y == (*b)->n-1) {
                // goes to next line
                if (futoshiki_simple(b, x+1, 0, calls)) {
                    // placement was successful
                    return TRUE;
                }
            } else {
                // goes to next column
                if (futoshiki_simple(b, x, y+1, calls)) {
                    // placement was sucessful
                    return TRUE;
                }
            }
        }
        // reset current value
        // don't change pre-filled values
        if (!((*b)->matrix[x][y].filled)) {
            (*b)->matrix[x][y].value = 0;
        }
    }
    // none of the placements were successful
    // something must be wrong, backtrack
    return FALSE;
}

// Backtrack with Forward Checking
bool futoshiki_fw(BOARD** b, int x, int y, int* calls) {
    return FALSE;
}

// Backtrack with Forward Checking + Minimal Remaining Values
bool futoshiki(BOARD** b, int x, int y) {
    return FALSE;
}

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
