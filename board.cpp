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
    clear();
}

inline bool Board::isEmpty() {
    return _empty;
}

inline void Board::clear() {
    for (int row = 0; row < _height; ++row)
        for (int col = 0; col < _width; ++col)
            _letters[row][col] = EMPTY_SQUARE;
    _empty = true;
}

void Board::writeColumnHeaders(ostream &o) {
    o << "  ";
    for (int col = 0; col < _width; ++col) o << " " << (char)('a' + col);
    o << endl;
}

void Board::writeHorizontalLine(ostream &o) {
    o << "   ";
    for (int i = 0; i < _width*2 - 1; ++i) o << "-";
    o << endl;
}

void Board::writeRows(ostream &o) {
    for (int row = 0; row < _height; ++row) writeRow(o, row);
}

void Board::writeRow(ostream &o, int row) {
    int displayRow = row + 1;
    if (displayRow < 10) o << " ";
    o << displayRow << "|";
    for (int col = 0; col < _width; ++col) {
        if (col) o << " ";
        writeSquare(o, row, col);
    }
    o << "|" << endl;
}

// doesn't yet handle blanks
void Board::writeLetter(ostream &o, int row, int col) {
    char displayLetter = (char)(_letters[row][col] - 1 + 'A');
    o << displayLetter;
}

void Board::writeSquare(ostream &o, int row, int col) {
    if (_letters[row][col]) writeLetter(o, row, col);
    else _layout->writeSquare(o, row, col);
}

ostream &operator<<(ostream &o, Board &board) {
    board.writeColumnHeaders(o);
    board.writeHorizontalLine(o);
    board.writeRows(o);
    board.writeHorizontalLine(o);
    board.writeColumnHeaders(o);
}
