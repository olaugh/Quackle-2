/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <vector>
#include "constants.h"
#include "types.h"
#include "util.h"
using namespace std;

class Move {
 public:
    enum Action {Place, Exchange, Pass};
    Move();
    Move(uint len, uchar *tiles, float equity);
    Move(uint len, uchar *tiles, uchar *letters, uint row, uint col, bool horiz,
         int score, float equity);

    bool isPass() const { return _action == Pass; }
    bool isExchange() const { return _action == Exchange; }
    bool isPlace() const { return _action == Place; }
    uint score() const { return _score; }
    float equity() const { return _equity; }

    void writePosition(ostream &o);
    void writeLetters(ostream &o);
    void writeTiles(ostream &o);
    
    static void sortByEquity(vector<Move> *list);
    static bool equityComparator(const Move &move1, const Move &move2);

 private:
    void writeRow(ostream &o);
    void writeCol(ostream &o);
    void writeLetter(ostream &o, uint idx);
    void writeTile(ostream &o, uint idx);

    uchar _letters[MAXIMUM_RACK_SIZE];
    uchar _tiles[MAXIMUM_RACK_SIZE];
    bool _horiz;
    uint _row;
    uint _col;
    uint _len;
    Action _action;
    int _score;
    float _equity;
};

ostream &operator<<(ostream &o, Move &move);

#endif
