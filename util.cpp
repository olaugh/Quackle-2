/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#include "util.h"
using namespace std;

void Util::writeBlankLetter(ostream &o, uchar letter) {
    o << (char)(letter + 'a');
}

void Util::writeLetter(ostream &o, uchar letter) {
    o << (char)(letter + 'A');
}

void Util::writeTile(ostream &o, uchar tile) {
    if (tile == BLANK) o << '?';
    else writeLetter(o, tile);
}
