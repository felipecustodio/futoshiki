#ifndef __BOARD_H__
#define __BOARD_H__


typedef struct board BOARD;
typedef struct cell CELL;
typedef struct list LIST;

struct list {

    // possible values
    int* vector;
    // number of possible values
    int size;
    // vector size is always board size

};

struct cell {

    // number of restrictions
    int r;
    // restriction coordinates
    int** restrictions;
    int value;
    bool filled; // determines if cell is pre-filled

    // possible values for use with heuristics
    LIST* fw;

};

struct board {

    // board size
    int n;
    CELL** matrix;

};

#endif
