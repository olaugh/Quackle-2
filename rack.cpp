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
#include "rack.h"
using namespace std;

Rack::Rack(uint len, uchar *tiles) {
    assert(len > 0 && len <= MAXIMUM_RACK_SIZE);
    memcpy(_tiles, tiles, len);
    _len = len;
    _capacity = DEFAULT_RACK_SIZE;
}

void Rack::writeTiles(ostream &o) {
    for (int i = 0; i < _len; ++i) writeTile(o, i);
}

void Rack::writeTile(ostream &o, uint idx) {
    Util::writeTile(o, _tiles[idx]);
}

void Rack::writeEmpties(ostream &o) {
    for (int i = 0 ; i < _capacity - _len; ++i) o << "_";
}

ostream &operator<<(ostream &o, Rack &rack) {
    rack.writeTiles(o);
    rack.writeEmpties(o);
    return o;
}
