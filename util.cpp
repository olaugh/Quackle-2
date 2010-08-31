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
    o << (char)(letter - FIRST_LETTER + 'a');
}

void Util::writeLetter(ostream &o, uchar letter) {
    o << (char)(letter - FIRST_LETTER + 'A');
}

void Util::writeTile(ostream &o, uchar tile) {
    if (tile == BLANK) o << '?';
    else writeLetter(o, tile);
}

uint Util::charToIndex(uchar c) {
    int i;
    if ((c >= 'a') && (c <= 'z')) return c + FIRST_LETTER - 'a';
    if ((c >= 'A') && (c <= 'Z')) return c + FIRST_LETTER - 'A';
    if ((c == '?') || (c == '.')) return i = BLANK;
    return EMPTY_SQUARE;
}

