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
#include "util.h"
using namespace std;

class Config {
 public:
    Config(const char *layout, const char *tiles);

    void writeSquare(ostream &o, int row, int col) const;
    inline uint startRow() const { return _startRow; }
    inline uint startCol() const { return _startCol; }
    inline uint tileScore(uint tile) const { return _scores[tile]; }
    inline uint lMul(uint row, uint col) const { return _lMul[row][col]; }
    inline uint wMul(uint row, uint col) const { return _wMul[row][col]; }

 private:
    void loadLayout(const char *filename);
    void loadTiles(const char *filename);
    bool checkDimensions();
    bool readGrid(ifstream &file);

    bool _valid;
    uint _width;
    uint _height;
    uint _startRow;
    uint _startCol;
    uint _lMul[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];
    uint _wMul[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];

    uint _counts[BLANK + 1];
    uint _scores[BLANK + 1];
};

#endif
