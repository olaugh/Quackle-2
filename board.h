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
    Board(int width, int height, Layout *layout);

    bool isEmpty();

 private:
    int _width;
    int _height;
    Layout *_layout;
    bool _empty;

    char m_letters[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];
};

ostream &operator<<(ostream &o, const Board &board);

#endif
