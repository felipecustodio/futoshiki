#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"

typedef struct list {

    int* vector; // list of possible values
    int size; // number of possible values
    int used; // check if position is used in MRV

} LIST;

typedef struct cell {

    int r; // number of restrictions
    int** restrictions; // restriction coordinates
    int value; // current value
    bool filled; // determines if cell is pre-filled
    LIST* fw; // list of possible values

} CELL;

typedef struct board {

    int n; // board dimensions
    CELL** matrix;

} BOARD;

/*
* list functions
*/
LIST* initList(LIST* list, int n) {
    int i;
    list = (LIST*)malloc(sizeof(LIST));
    // list has n members
    list->vector = (int*)malloc(sizeof(int) * n);
    // add possible values to list
    for (i = 0; i < n; i++) {
        list->vector[i] = i + 1;
    }
    list->size = n;
    list->used = 0;
    return list;
}

LIST* listRemove(LIST* list, int value, int n) {
    list->vector[value-1] = 0;
    return list;
}

LIST* listAdd(LIST* list, int value, int n) {
    list->vector[value-1] = value;
    list->size++;
    return list;
}

BOARD* updateLists(BOARD* b, int x, int y, int flag, int value) {
    int i;
    // flag 0: remove value
    // flag 1: add value
    if (flag) {
        // add current value from cells that conflict with current cell
        for (i = 0; i < b->n; i++) {
            // check horizontally
            if (i != y) {
                b->matrix[x][i].fw = listAdd(b->matrix[x][i].fw, value, b->n);
            }
            // check vertically
            if (i != x) {
                b->matrix[i][y].fw = listAdd(b->matrix[i][y].fw, value, b->n);
            }
        }
    } else {
        // set number of possible values to over n, so it's never found as MRV
        b->matrix[x][y].fw->size = b->n + 1;
        // remove current value from cells that conflict with current cell
        for (i = 0; i < b->n; i++) {
            // check horizontally
            if (i != y) {
                b->matrix[x][i].fw = listRemove(b->matrix[x][i].fw, value, b->n);
            }
            // check vertically
            if (i != x) {
                b->matrix[i][y].fw = listRemove(b->matrix[i][y].fw, value, b->n);
            }
        }
    }
    return b;
}

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
            board->matrix[i][j].fw = initList(board->matrix[i][j].fw, n);
        }
    }
    return board;
}

// free memory
BOARD* destroyBoard(BOARD* board) {
    int i, j, k;
    int aux;
    for (i = 0; i < board->n; i++) {
        // free columns (cells)
        for (j = 0; j < board->n; j++) {
            // free restriction list for current cell
            aux = board->matrix[i][j].r;
            for (k = 0; k < aux; k++) {
                free(board->matrix[i][j].restrictions[k]);
                board->matrix[i][j].restrictions[k] = NULL;
            }
            free(board->matrix[i][j].restrictions);
            board->matrix[i][j].restrictions = NULL;
        }
        // free line
        free(board->matrix[i]);
        board->matrix[i] = NULL;
    }
    // free board
    free(board->matrix);
    board->matrix = NULL;
    free(board);
    board = NULL;
    return board;
}

// READ BOARDS
BOARD** readBoards(int n) {
    int i, j, k;
    BOARD** b = NULL;
    // board dimensions, number of restrictions
    int d, r;
    // restriction coordinates
    int x1, y1, x2, y2;

    // allocate a vector of boards
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
                // read current value
                scanf("%d", &(b[i]->matrix[j][k].value));
                // checks if value is predefined
                if (b[i]->matrix[j][k].value != 0) {
                    // if value is predefined, we need to update the forward checking lists
                    b[i]->matrix[j][k].filled = TRUE;
                    b[i]->matrix[j][k].fw->used = 1;
                    // update FW lists for this column and line
                    b[i] = updateLists(b[i], j, k, 0, b[i]->matrix[j][k].value);
                }
            }
        }

        // read restrictions
        for (j = 0; j < r; j++) {
            // get restriction coordinates
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            // add more restrictions for current position
            b[i]->matrix[x1-1][y1-1].restrictions = (int**)realloc(b[i]->matrix[x1-1][y1-1].restrictions, sizeof(int*) * b[i]->matrix[x1-1][y1-1].r + 1);
            // allocate memory for x,y coordinates
            b[i]->matrix[x1-1][y1-1].restrictions[b[i]->matrix[x1-1][y1-1].r] = (int*)malloc(sizeof(int) * 2);
            // update coordinates to indices
            b[i]->matrix[x1-1][y1-1].restrictions[b[i]->matrix[x1-1][y1-1].r][0] = x2-1;
            b[i]->matrix[x1-1][y1-1].restrictions[b[i]->matrix[x1-1][y1-1].r][1] = y2-1;
            // increase number of restrictions
            b[i]->matrix[x1-1][y1-1].r += 1;
        }
    }
    // all boards read successfully
    return b;
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

bool _isValid(BOARD* board, int x, int y) {
    int i;
    int bigger; // store the value in the constraint coordinate

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

int getListSize(LIST* list, int n) {
    int i;
    int size = 0;
    for (i = 0; i < n; i++) {
        if (list->vector[i] != 0) {
            size++;
        }
    }
    return size;
}


// Prints size of forward checking lists
void printMRV(BOARD *b) {
    int i, j;
    for (i = 0; i < b->n; i++) {
        for (j = 0; j < b->n; j++) {
            printf("%d ", getListSize(b->matrix[i][j].fw, b->n));
        }
        printf("\n");
    }
    printf("\n\n");
}

// Find position with smallest number of possible values
void findMRV(BOARD* b, int _x, int _y, int* x, int* y) {
    int i, j;
    int aux = b->n + 1;
    int tmp = 0;
    *x = _x;
    *y = _y;
    // search for smallest value
    for (i = 0; i < b->n; i++) {
        for (j = 0; j < b->n; j++) {
            tmp = getListSize(b->matrix[i][j].fw, b->n);
            if (tmp < aux && !(b->matrix[i][j].filled) && !(b->matrix[i][j].fw->used)) {
                aux = tmp;
                *y = j;
                *x = i;
            }
        }
    }
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

// BACKTRACK WITH FORWARD CHECKING HEURISTIC
bool futoshiki_fw(BOARD** b, int x, int y, int* calls) {
    int i;
    // check if recursive calls reached overflow
    if (*calls >= OVERFLOW) {
        return FALSE;
    }
    // check if has reached end of board
    if (x >= (*b)->n || y >= (*b)->n) {
        return TRUE;
    }
    // check if position is free and domain is not empty
        for (i = 0; i < (*b)->n; i++) {
            if ((*b)->matrix[x][y].value == 0 && (*b)->matrix[x][y].fw->size > 0) {
            // get first valid value in forward checking list
            if ((*b)->matrix[x][y].fw->vector[i] != 0) {
                // don't overwrite prefilled values
                if (!((*b)->matrix[x][y].filled)) {
                    (*b)->matrix[x][y].value = (*b)->matrix[x][y].fw->vector[i];
                    (*b)->matrix[x][y].fw->used = 1;
                    (*calls)++;
                    // update forward checking lists (remove current value) of line and column
                    *b = updateLists(*b, x, y, 0, (*b)->matrix[x][y].value);
                }
                if (isValid(*b, x, y)) {
                    // checks if reached last column
                    if (y == (*b)->n-1) {
                        // goes to next line
                        if (futoshiki_fw(b, x+1, 0, calls)) {
                            // placement was successful
                            return TRUE;
                        }
                    } else {
                        // goes to next column
                        if (futoshiki_fw(b, x, y+1, calls)) {
                            // placement was sucessful
                            return TRUE;
                        }
                    }
                }
            }
            // reset value
            if (!((*b)->matrix[x][y].filled)) {
                // reset lists
                *b = updateLists(*b, x, y, 1, (*b)->matrix[x][y].value);
                // reset value
                (*b)->matrix[x][y].value = 0;
                (*b)->matrix[x][y].fw->used = 0;
            }
        } else {
            // checks if reached last column
            if (y == (*b)->n-1) {
                // goes to next line
                if (futoshiki_fw(b, x+1, 0, calls)) {
                    // placement was successful
                    return TRUE;
                }
            } else {
                // goes to next column
                if (futoshiki_fw(b, x, y+1, calls)) {
                    // placement was sucessful
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

bool futoshiki(BOARD** b, int x, int y, int* calls, int* counter) {
    int i;
    int _x = x;
    int _y = y;

    // check if recursive calls reached overflow
    if (*calls >= OVERFLOW) {
        return FALSE;
    }

    // verifies if complete
    if ((*counter) >= (*b)->n * (*b)->n + 1) {
        return TRUE;
    }

    for (i = 0; i < (*b)->n; i++) {

        if ((*b)->matrix[x][y].fw->vector[i] > 0) {
            (*b)->matrix[x][y].value = (*b)->matrix[x][y].fw->vector[i];
            (*calls)++;
            (*b)->matrix[x][y].fw->used = 1;
            (*counter)++;
            if (isValid(*b, x, y)) {
                updateLists(*b, x, y, 0, (*b)->matrix[x][y].value);
                findMRV(*b, x, y, &_x, &_y);
                if (futoshiki(b, _x, _y, calls, counter)) {
                    return TRUE;
                }
            }
            // reset
            (*counter)--;
            (*b)->matrix[x][y].fw->used = 0;
            updateLists(*b, x, y, 1, (*b)->matrix[x][y].value);
            (*b)->matrix[x][y].value = 0;
        }
    }
    return FALSE;
}

int main(int argc, char const *argv[]) {

    // benchmarking
    clock_t start_t, end_t;
    float delta_t = 0.0;
    int calls;
    int solved = 0;

    system("clear");
    printf("\tFUTOSHIKI 不等式\n\n");
    int i;
    int n = 0;
    int menu = 0;
    int counter = 1;
    scanf("%d", &menu);
    scanf("%d", &n);
    BOARD** boards = readBoards(n);
    // start timer
    start_t = clock();
    switch(menu) {
        printf("::: Change first .dat file number to menu option\n");
        printf("1 :: Bruteforce\n");
        printf("2 :: Forward Checking\n");
        printf("3 :: Forward Checking + MRV\n");
        case 1:
            printf("Running boards with bruteforce...\n");
            for (i = 0; i < n; i++) {
                calls = 0;
                printf("::: Board %d\n", i+1);
                if (futoshiki_simple(&boards[i], 0, 0, &calls)) {
                    solved++;
                    printf(":: Solved in %d calls\n", calls);
                    printBoard(boards[i]);
                } else {
                    if (calls >= OVERFLOW) {
                        printf("::: ｏｖｅｒｆｌｏｗ\n");
                        printf(":: %d calls.\n", calls);
                    }
                    printf("[ ! ] No solution\n");
                }
                printf("\n");
            }
            // end timer
            end_t = clock();
            // human readable time
            delta_t = ((float)(end_t - start_t) / 1000000000000.0F ) * CLOCKS_PER_SEC;
            printf("\nFINISHED.\n");
            printf("Solved %d/%d boards in %lf seconds.\n\n", solved, n, delta_t);
            break;
        case 2:
            printf("Running boards with forward checking...\n");
            for (i = 0; i < n; i++) {
                calls = 0;
                printf("::: Board %d\n", i+1);
                if (futoshiki_fw(&boards[i], 0, 0, &calls)) {
                    solved++;
                    printf(":: Solved in %d calls\n", calls);
                    printBoard(boards[i]);
                } else {
                    if (calls >= OVERFLOW) {
                        printf("::: ｏｖｅｒｆｌｏｗ\n");
                        printf(":: %d calls.\n", calls);
                    }
                    printf("[ ! ] No solution\n");
                }
                printf("\n");
            }
            // end timer
            end_t = clock();
            // human readable time
            delta_t = ((float)(end_t - start_t) / 1000000000000.0F ) * CLOCKS_PER_SEC;
            printf("\nFINISHED.\n");
            printf("Solved %d/%d boards in %lf seconds.\n\n", solved, n, delta_t);
            break;
        case 3:
            printf("Running boards with forward checking + MRV\n");
            for (i = 0; i < n; i++) {
                calls = 0;
                counter = 1;
                printf("::: Board %d\n", i+1);
                if (futoshiki(&boards[i], 0, 0, &calls, &counter)) {
                    solved++;
                    printf(":: Solved in %d calls\n", calls);
                    printBoard(boards[i]);
                } else {
                    if (calls >= OVERFLOW) {
                        printf("::: ｏｖｅｒｆｌｏｗ\n");
                        printf(":: %d calls.\n", calls);
                    }
                    printf("[ ! ] No solution\n");
                }
                printf("\n");
                boards[i] = destroyBoard(boards[i]);
            }
            // end timer
            end_t = clock();
            // human readable time
            delta_t = ((float)(end_t - start_t) / 1000000000000.0F ) * CLOCKS_PER_SEC;
            printf("\nFINISHED.\n");
            printf("Solved %d/%d boards in %lf seconds.\n\n", solved, n, delta_t);
    }
    return 0;
}
