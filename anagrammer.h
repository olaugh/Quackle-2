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

class Anagrammer
{
 public:
    Anagrammer(char* dict);
    void anagram(const char* input);

 private:
    uint mask[32];
    const uint *dawg;

    inline char toChar(unsigned int index);
    inline void printLowercase(uchar *perm, int length);
    inline void printTruncated(uchar *permutation, int length);
    inline bool hasChild(const uint *dawg, uint nodeIndex, uchar c);
    inline unsigned int getChild(const uint *dawg, uint nodeIndex, char c);
    inline bool terminates(uint node);
    inline unsigned int getPointer(uint node);
    inline bool skipAhead(uchar *perm, int start);
    inline int countTiles(const char *input, uint *counts);

    const unsigned int* loadDawg(char *filename);
    void computeMasks();
};

#endif
