/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include "constants.h"
#include "types.h"
using namespace std;

class Util
{
 public:
    static void writeBlankLetter(ostream &o, uchar letter);
    static void writeLetter(ostream &o, uchar letter);
    static void writeTile(ostream &o, uchar tile);
    static uint charToIndex(uchar c);
};

#endif
