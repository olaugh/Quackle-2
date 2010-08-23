/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef ANAGRAMMER_H
#define ANAGRAMMER_H

#include "board.h"
#include "constants.h"
#include "move.h"
#include "types.h"
#include "rack.h"
#include "util.h"

class Anagrammer
{
 public:
    Anagrammer(const char* dict);
    void anagram(const char* input);
    void findMoves(const Board &board, const Rack rack);

 private:
    uint _mask[32];
    const uint *_dawg;
    uchar _perm[MAXIMUM_RACK_SIZE + 1];

    inline char toChar(unsigned int index);
    inline void printTruncated(int length);
    inline bool hasChild(uint nodeIndex, uchar c);
    inline unsigned int getChild(uint nodeIndex, char c);
    inline bool terminates(uint node);
    inline unsigned int getPointer(uint node);
    inline bool skipAhead(int start);
    inline int countTiles(const char *input, uint *counts);

    const unsigned int* loadDawg(const char *filename);
    void computeMasks();
};

#endif
