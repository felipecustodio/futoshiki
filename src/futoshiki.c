#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"
#include "board.h"
#include "futoshiki.h"

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

//Forward checcking
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
                    (*calls)++;
                    // update forward checking lists (remove current value) of line and column
                    updateLists(b, x, y, 0, (*b)->matrix[x][y].value);
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
                updateLists(b, x, y, 1, (*b)->matrix[x][y].value);
                // reset value
                (*b)->matrix[x][y].value = 0;
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


// Backtrack with Forward Checking + Minimal Remaining Values
bool futoshiki(BOARD** b, int x, int y) {
    return FALSE;
}
