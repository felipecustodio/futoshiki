#ifndef __FUTOSHIKI_H__
#define __FUTOSHIKI_H__

//Backtracking functions
bool futoshiki_simple(BOARD** b, int x, int y, int* calls);
bool futoshiki_fw(BOARD** b, int x, int y, int* calls);
bool futoshiki(BOARD** b, int x, int y);

#endif
