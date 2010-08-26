/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#include <assert.h>
#include <string.h>
#include "move.h"
using namespace std;

Move::Move() {
    _action = Pass;
    _len = 0;
    _score = 0;
}

Move::Move(uint len, uchar *tiles) {
    _action = Exchange;
    assert(len > 0 && len <= MAXIMUM_RACK_SIZE);
    memcpy(_tiles, tiles, len);
    _len = len;
    _score = 0;
}

Move::Move(uint len, uchar *tiles, uchar *letters, uint row, uint col,
           bool horiz, int score, float equity) {
    _action = Place;
    assert(len > 0 && len <= MAXIMUM_RACK_SIZE);
    memcpy(_tiles, tiles, len);
    memcpy(_letters, letters, len);
    _row = row;
    _col = col;
    _horiz = horiz;
    _len = len;
    _score = score;
    _equity = equity;
}

void Move::writePosition(ostream &o) {
    if (_horiz) {
        writeRow(o);
        writeCol(o);
    } else {
        writeCol(o);
        writeRow(o);
    }
}

void Move::writeRow(ostream &o) {
    uint displayRow = _row + 1;
    o << displayRow;
}

void Move::writeCol(ostream &o) {
    uchar displayCol = (uchar)(_col + 'a');
    o << displayCol;
}

void Move::writeLetters(ostream &o) {
    for (int i = 0; i < _len; ++i) writeLetter(o, i);
}

void Move::writeLetter(ostream &o, uint idx) {
    char tile = _tiles[idx];
    char letter = _letters[idx];
    if (tile == BLANK) Util::writeBlankLetter(o, letter);
    else Util::writeLetter(o, letter);
}

void Move::writeTiles(ostream &o) {
    for (int i = 0; i < _len; ++i) writeTile(o, i);
}

void Move::writeTile(ostream &o, uint idx) {
    Util::writeTile(o, _tiles[idx]);
}

ostream &operator<<(ostream &o, Move &move) {
    if (move.isPass()) {
        o << "Pass";
    } else if (move.isExchange()) {
        o << "Exch ";
        move.writeTiles(o);
    } else {
        move.writePosition(o);
        o << " ";
        move.writeLetters(o);
    }
    return o;
}
