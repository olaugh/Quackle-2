/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef RACK_H
#define RACK_H

#include "constants.h"
#include "types.h"
#include "util.h"
using namespace std;

class Rack {
 public:
    Rack(uint len, uchar *tiles);

    void writeTiles(ostream &o);
    void writeEmpties(ostream &o);
    inline uint len() const { return _len; }
    inline uint tile(uint i) const { return _tiles[i]; }

 private:
    void writeTile(ostream &o, uint idx);
    uchar _tiles[MAXIMUM_RACK_SIZE];
    uint _capacity;
    uint _len;
};

ostream &operator<<(ostream &o, Rack &rack);

#endif
