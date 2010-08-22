/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "board.h"
using namespace std;

Board::Board(int width, int height, Layout *layout) {
    _width = width;
    _height = height;
    _layout = layout;
    _empty = true;
}

inline bool Board::isEmpty() {
    return _empty;
}

ostream &operator<<(ostream &o, const Board &board) {
    o << "<Board>" << endl;
}
