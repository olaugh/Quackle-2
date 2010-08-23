/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef BOARD_H
#define BOARD_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "layout.h"
using namespace std;

class Board
{
 public:
    Board(int width, int height, const Layout *layout);

    void clear();
    inline bool isEmpty() const { return _empty; }
    inline const Layout* layout() const { return _layout; }
    void writeColumnHeaders(ostream &o);
    void writeHorizontalLine(ostream &o);
    void writeRows(ostream &o);
    void writeRow(ostream &o, int row);
    void writeLetter(ostream &o, int row, int col);
    void writeSquare(ostream &o, int row, int col);
    
 private:
    int _width;
    int _height;
    const Layout *_layout;
    bool _empty;

    char _letters[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];
};

ostream &operator<<(ostream &o, Board &board);

#endif
