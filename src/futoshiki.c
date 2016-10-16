#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"

typedef struct list {

    int* list;
    int size;

} LIST;

typedef struct cell {

    // number of restrictions
    int r;
    // restriction coordinates
    int** restrictions;
    int value;
    bool filled; // determines if cell is pre-filled

} CELL;

typedef struct board {

    // board size
    int n;
    CELL** matrix;

} BOARD;

/*
*
*/



/*
* board functions
*/
void printBoard(BOARD* board) {
    int i, j;
    for (i = 0; i < board->n; i++) {
        for (j = 0; j < board->n; j++) {
            printf("[%d]", board->matrix[i][j].value);
        }
        printf("\n");
    }
}

BOARD* initBoard(int n) {
    int i, j;
    BOARD* board = (BOARD*)malloc(sizeof(BOARD));
    board->n = n;
    board->matrix = (CELL**)malloc(sizeof(CELL*) * n);
    for (i = 0; i < n; i++) {
        board->matrix[i] = (CELL*)malloc(sizeof(CELL) * n);
        for (j = 0; j < n; j++) {
            board->matrix[i][j].value = -1;
            board->matrix[i][j].r = 0;
            board->matrix[i][j].restrictions = NULL;
            board->matrix[i][j].filled = FALSE;
        }
    }
    return board;
}

// TODO: This is probably very wrong
BOARD* destroyBoard(BOARD* board) {
    int i, j, k;
    int aux;
    for (i = 0; i < board->n; i++) {
        // percorrer colunas
        for (j = 0; j < board->n; j++) {
            // liberar restrictions de [i][j]
            aux = board->matrix[i][j].r;
            for(k = 0; k < aux; k++) {
                free(board->matrix[i][j].restrictions[k]);
                board->matrix[i][j].restrictions[k] = NULL;
            }
            free(board->matrix[i][j].restrictions);
            board->matrix[i][j].restrictions = NULL;
        }
        free(board->matrix[i]);
        board->matrix[i] = NULL;
    }
    free(board->matrix);
    board->matrix = NULL;
    free(board);
    board = NULL;
    return board;
}

/*
* game functions
*/
bool isValid(BOARD* board, int x, int y) {
    int i;
    int bigger; // store the value in the constraint coordinate
    int value = board->matrix[x][y].value;

    // check if value already exists
    for (i = 0; i < board->n; i++) {
        // check horizontally
        if (i != y && board->matrix[x][i].value == value) {
            return FALSE;
        }
        // check vertically
        if (i != x && board->matrix[i][y].value == value) {
            return FALSE;
        }
    }

    // check if position has constraint
    if (board->matrix[x][y].restrictions != NULL) {
        // check constraints
        for (i = 0; i < board->matrix[x][y].r; i++) {
            bigger = board->matrix[board->matrix[x][y].restrictions[i][0]][board->matrix[x][y].restrictions[i][1]].value;
            if (bigger > 0 && board->matrix[x][y].value > bigger) {
                return FALSE;
            }
        }
    }

    // all conditions were valid
    return TRUE;
}

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
        // check if value isn't already used
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

// Backtrack with MRV

// Backtrack with Forward Checking

// Backtrack with Minimal Conflicts

// READ BOARDS
BOARD** readBoards(int n) {
    int i, j, k;
    BOARD** b = NULL;
    int d, r;
    int x1, y1, x2, y2;

    b = (BOARD**)malloc(sizeof(BOARD*) * n);
    for (i = 0; i < n; i++) {
        // get size of board
        scanf("%d", &d);
        // allocate memory for new board
        b[i] = initBoard(d);
        // get number of restrictions
        scanf("%d", &r);
        // read board
        for (j = 0; j < d; j++) {
            for (k = 0; k < d; k++) {
                scanf("%d", &(b[i]->matrix[j][k].value));
                if (b[i]->matrix[j][k].value != 0) {
                    b[i]->matrix[j][k].filled = TRUE;
                }
            }
        }
        // read restrictions
        for (j = 0; j < r; j++) {
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            b[i]->matrix[x1-1][y1-1].restrictions = (int**)realloc(b[i]->matrix[x1-1][y1-1].restrictions, sizeof(int*) * b[i]->matrix[x1-1][y1-1].r + 1);
            b[i]->matrix[x1-1][y1-1].restrictions[b[i]->matrix[x1-1][y1-1].r] = (int*)malloc(sizeof(int) * 2);
            b[i]->matrix[x1-1][y1-1].restrictions[b[i]->matrix[x1-1][y1-1].r][0] = x2-1;
            b[i]->matrix[x1-1][y1-1].restrictions[b[i]->matrix[x1-1][y1-1].r][1] = y2-1;
            b[i]->matrix[x1-1][y1-1].r += 1;
        }
    }
    return b;
}

int main(int argc, char const *argv[]) {

    printf("\tFUTOSHIKI 不等式\n\n");
    int i;
    int calls;
    int n = 0;
    scanf("%d", &n);
    BOARD** boards = readBoards(n);
    for (i = 0; i < n; i++) {
        calls = 0;
        printf("::: Board %d\n", i+1);
        if (futoshiki_simple(&boards[i], 0, 0, &calls)) {
            printf("Took %d calls\n", calls);
            printBoard(boards[i]);
        } else {
            if (calls >= OVERFLOW) {
                printf("Overflow! :: %d calls.\n", calls);
            }
            printf("Can't solve this board!\n");
        }
        printf("\n");
    }

    for (i = 0; i < n; i++) {
        printf("Liberando tabuleiro #%d\n", i);
        boards[i] = destroyBoard(boards[i]);
        if (boards[i] == NULL) {
            printf("Liberado\n");
        } else {
            printf("Não liberado\n");
        }
    }
    free(boards);

    return 0;
}
