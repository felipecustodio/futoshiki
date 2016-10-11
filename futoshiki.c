#include <stdio.h>
#include <stdlib.h>

// globals.h
#define bool char
#define TRUE 1
#define FALSE 0

typedef struct cell {

    // restriction coordinates
    int x, y;
    int value;

} CELL;

typedef struct board {

    int n;
    CELL** matrix;

} BOARD;

/*
* board functions
*/
BOARD* initBoard(int n) {
    int i, j;
    BOARD* board = (BOARD*)malloc(sizeof(BOARD));
    board->matrix = (CELL**)malloc(sizeof(CELL*) * n);
    for (i = 0; i < n; i++) {
        board->matrix[i] = (CELL*)malloc(sizeof(CELL) * n);
        for (j = 0; j < n; j++) {
            board->matrix[i][j].value = 0;
            board->matrix[i][j].x = 0;
            board->matrix[i][j].y = 0;
        }
    }
    return board;
}

void printBoard(BOARD* board) {
    int i, j;
    for (i = 0; i < board->n; i++) {
        for (j = 0; j < board->n; j++) {
            printf("[%d]", board->matrix[i][j].value);

        }
        printf("\n");
    }
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
        if (i != y && board->matrix[x][i].value == value) return FALSE;
        // check vertically
        if (i != x && board->matrix[i][y].value == value) return FALSE;
    }
    // check if value respects constraint
    bigger = board->matrix[board->matrix[x][y].x][board->matrix[x][y].y].value;

    if (bigger > 0 && board->matrix[x][y].value > bigger) {
        return FALSE;
    }

    // all conditions were valid
    return TRUE;
}

int main(int argc, char const *argv[]) {

    int i, j, k;
    int n, d, r;
    int x1, y1, x2, y2;
    BOARD* board = NULL;
    printf("Number of tests: ");
    scanf("%d", &n); // number of tests to run

    // tests cycle
    for (i = 0; i < n; i++) {

        // reset board
        if (board != NULL) {
            free(board);
            board = NULL;
        }

        scanf("%d", &d); // board dimensions
        BOARD* board = initBoard(d);
        scanf("%d", &r); // number of restrictions
        // read board
        for (j = 0; j < d; j++) {
            for (k = 0; k < d; k++) {
                scanf("%d", &(board->matrix[j][k].value));
            }
        }
        // read restrictions
        for (j = 0; j < r; j++) {
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            board->matrix[x1][y1].x = x2;
            board->matrix[x1][y1].y = y2;
        }
    }

    return 0;
}
