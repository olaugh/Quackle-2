/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "constants.h"
#include "types.h"
using namespace std;

class Config {
 public:
    Config(const char *filename);

    void writeSquare(ostream &o, int row, int col) const;
    inline uint startRow() const { return _startRow; }
    inline uint startCol() const { return _startCol; }

 private:
    bool checkDimensions();
    bool readGrid(ifstream &file);

    bool _valid;
    uint _width;
    uint _height;
    uint _startRow;
    uint _startCol;
    uint _lMul[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];
    uint _wMul[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];
};

#endif
