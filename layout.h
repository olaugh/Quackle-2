/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef LAYOUT_H
#define LAYOUT_H

#include "constants.h"

using namespace std;

class Layout {
 public:
    Layout(char *filename);

    void writeSquare(ostream &o, int row, int col);

 private:
    bool checkDimensions();
    bool readGrid(ifstream &file);

    bool _valid;
    int _width;
    int _height;
    int _startRow;
    int _startCol;
    int _lMul[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];
    int _wMul[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];
};

#endif
