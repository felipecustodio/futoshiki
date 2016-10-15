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
void printBoard(BOARD* board) {
    int i, j;
    for (i = 0; i < board->n; i++) {
        for (j = 0; j < board->n; j++) {
            printf("[%d]", board->matrix[i][j].value);
        }
        printf("\n");
    }
}

BOARD* initBoard(BOARD* board, int n) {
    int i, j;
    board = (BOARD*)malloc(sizeof(BOARD));
    board->n = n;
    board->matrix = (CELL**)malloc(sizeof(CELL*) * n);
    for (i = 0; i < n; i++) {
        board->matrix[i] = (CELL*)malloc(sizeof(CELL) * n);
        for (j = 0; j < n; j++) {
            board->matrix[i][j].value = -1;
            board->matrix[i][j].x = -1;
            board->matrix[i][j].y = -1;
        }
    }
    return board;
}

BOARD* destroyBoard(BOARD* board) {
    int i;
    for (i = 0; i < board->n; i++) {
        free(board->matrix[i]);
        board->matrix[i] = NULL;
    }
    free(board->matrix);
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
    if (board->matrix[x][y].x != -1 && board->matrix[x][y].y != -1) {
        // check if value respects constraint
        bigger = board->matrix[board->matrix[x][y].x][board->matrix[x][y].y].value;
        if (bigger >= 0 && board->matrix[x][y].value > bigger) {
            return FALSE;
        }
    }

    // all conditions were valid
    return TRUE;
}

// BACKTRACKING - SIMPLE, NO HEURISTICS
bool futoshiki_simple(BOARD** b, int x, int y) {
    int i;
    // check if has reached end of board
    if (x >= (*b)->n || y >= (*b)->n) {
        return TRUE;
    }
    // try possible values
    for (i = 1; i <= (*b)->n; i++) {
        // check if cell is occupied
        if ((*b)->matrix[x][y].value == 0) {
            (*b)->matrix[x][y].value = i;
        }
        // check if value isn't already used
        if (isValid(*b, x, y)) {
            // checks if reached last column
            if (y == (*b)->n-1) {
                // goes to next line
                if (futoshiki_simple(b, x+1, 0)) {
                    // placement was successful
                    return TRUE;
                }
            } else {
                // goes to next column
                if (futoshiki_simple(b, x, y+1)) {
                    // placement was sucessful
                    return TRUE;
                }
            }
        }
        // reset current value
        (*b)->matrix[x][y].value = 0;
    }
    // none of the placements were successful
    // something must be wrong, backtrack
    return FALSE;
}

// Backtrack with MRV

// Backtrack with Forward Checking

// Backtrack with Minimal Conflicts

int main(int argc, char const *argv[]) {
    int i, j, k;
    // input
    int n, d, r;
    // restriction coordinates
    int x1, y1, x2, y2;
    // futoshiki board
    BOARD* board = NULL;

    scanf("%d", &n); // number of tests to run
    // tests cycle
    for (i = 0; i < n; i++) {
        printf("Test Cycle #%d\n", i);

        // reset board
        if (board != NULL) {
            printf("Resetting board\n");
            board = destroyBoard(board);
            if (board == NULL) {
                printf("Destruction successful\n");
            }
        }

        scanf("%d", &d); // board dimensions
        board = initBoard(board, d); // allocate memory
        scanf("%d", &r); // number of restrictions
        printf("Board of size %d with %d restrictions\n", d, r);

        // read board
        for (j = 0; j < d; j++) {
            for (k = 0; k < d; k++) {
                scanf("%d", &(board->matrix[j][k].value));
            }
        }

        // read restrictions
        for (j = 0; j < r; j++) {
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            board->matrix[x1-1][y1-1].x = x2-1;
            board->matrix[x1-1][y1-1].y = y2-1;
        }

        printf("Board:\n");
        printBoard(board);

        // solve current board
        printf("Starting Backtracking\n");
        if (futoshiki_simple(&board, 0, 0)) {
            printf("%d\n", i);
            printBoard(board);
        } else {
            printf("NO SOLUTION!\n");
        }
        printf("\n");
    }

    return 0;
}
