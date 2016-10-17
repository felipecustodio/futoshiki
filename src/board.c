#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"


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
    return list;
}