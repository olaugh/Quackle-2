/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include "anagrammer.h"
using namespace std;

Anagrammer::Anagrammer(const char* dict) {
    _dawg = loadDawg(dict);
    computeMasks();
}

inline void Anagrammer::printTruncated(int length) {
    for (int i = 0; i < length; ++i) Util::writeTile(cout, _perm[i]);
    cout << endl;
}

inline bool Anagrammer::hasChild(uint nodeIndex, uchar c) {
    uint bits = _dawg[nodeIndex];
    uint bit = 1 << c;
    return (bits & bit) != 0;
}

inline uint Anagrammer::getChild(uint nodeIndex, char c) {
    uint bits = _dawg[nodeIndex];
    uint n = c;
    return _dawg[nodeIndex + 1 + __builtin_popcount(bits & _mask[n])];
}

inline bool Anagrammer::terminates(unsigned int node) {
    return (node & 1) != 0;
}

inline unsigned int Anagrammer::getPointer(unsigned int node) {
    return node >> 8;
}

inline bool Anagrammer::skipAhead(int start) {
    for (int i = start + 1; _perm[i] != '\0'; ++i) {
        if (_perm[i] > _perm[start]) {
            swap(_perm[start], _perm[i]);
            return true;
        }
    }
 
    char temp = _perm[start];
    int numOfTemp = 1;
    for (uchar* dupPtr = _perm + start + 1; dupPtr[0] == temp; ++dupPtr) {
        ++numOfTemp;
    }

    uchar* cyclePtr = _perm + start;
    while (cyclePtr[numOfTemp] != '\0') {
        cyclePtr[0] = cyclePtr[numOfTemp];
        ++cyclePtr;
    }
    for (int i = 0; i < numOfTemp; ++i) cyclePtr[i] = temp;
 
    return false;
}

inline int Anagrammer::countTiles(const char *input, uint *counts) {
    uint validChars = 0;
    for (int i = 0; i <= BLANK; i++) counts[i] = 0;

    for (const char* p = input; p[0] != '\0'; ++p) {
        char c = p[0];
        int i;
        bool valid = true;

        if ((c >= 'a') && (c <= 'z')) {
            i = c + FIRST_LETTER - 'a';
        } else if ((c >= 'A') && (c <= 'Z')) {
            i = c + FIRST_LETTER - 'A';
        } else if ((c == '?') || (c == '.')) {
            i = BLANK;
        } else {
            cout << "invalid character " << c << " in " << input << endl;
            valid = false;
        }

        if (valid) {
            ++counts[i];
            ++validChars;
        }
    }
    return validChars;
}

uint Anagrammer::setFirstPerm(const char *input) {
    uint counts[BLANK + 1];
    uint validChars = countTiles(input, counts);
    if (!validChars) {
        cout << "no valid input" << endl;
        return 0;
    }

    /* This is an implementation of Algorithm L (Lexicographic permutation
       generation) from Knuth's TAOCP Volume 4, Chapter 7.2.1.2. */
    // L1. [Visit a_1,a_2...a_n]
    uint index = 0;
    for (int tile = 0; tile < NUM_TILES; ++tile)
        for (int j = 0; j < counts[tile]; ++j) _perm[index++] = tile;
    _perm[index] = '\0';

    return validChars;
}

void Anagrammer::findMoves(const Board &board, const Rack rack) {

}

void Anagrammer::anagram(const char *input) {
    int validChars = setFirstPerm(input);
    if (!validChars) return;
    _nodes[0] = 0;

    uint newestPrefixLen = 1;
    uint skipUntilNewAt = 1;
    for (;;) {
        if (newestPrefixLen <= skipUntilNewAt + 1) {
            for (int i = newestPrefixLen; i <= validChars; ++i) {
                if (hasChild(_nodes[i - 1], _perm[i - 1])) {
                    uint child = getChild(_nodes[i - 1], _perm[i - 1]);
                    if (terminates(child)) printTruncated(i);
                    unsigned int newNode = getPointer(child);
                    _nodes[i] = newNode;
                    if (!newNode) {
                        skipUntilNewAt = i - 1;
                        break;
                    }
                } else {
                    skipUntilNewAt = i - 1;
                    break;
                }
            }
        }

        // L2. [Find j]
        int j = validChars - 2;
        while (_perm[j] >= _perm[j + 1]) {
            if (j == 0) return;
            --j;
        }

        if (j > skipUntilNewAt) {
            while (!skipAhead(skipUntilNewAt)) {
                if (!skipUntilNewAt) return;
                --skipUntilNewAt;
            }
            newestPrefixLen = skipUntilNewAt + 1;
        } else {
            // L3. [Increase a_j]
            int l = validChars - 1;
            while (_perm[j] >= _perm[l]) --l;
            swap(_perm[j], _perm[l]);
            newestPrefixLen = j + 1;

            // L4. [Reverse a_j+1.,.a_n]
            int r0 = j + 1;
            int r1 = validChars - 1;
            while (r0 < r1) swap(_perm[r0++], _perm[r1--]);
        }
    }
}

const uint* Anagrammer::loadDawg(const char *filename) {
    ifstream file(filename);

    char fileType[5];
    file.read(fileType, 4);
    fileType[4] = '\0';
    //cout << fileType << endl;

    uint sizeBytes[1];
    file.read((char*)sizeBytes, 4);
    uint dawgNodes = sizeBytes[0];

    char *dawg = new char[dawgNodes * 4];
    file.read(dawg, dawgNodes * 4);
    return (uint*)dawg;
}

void Anagrammer::computeMasks() {
    _mask[0] = 0;
    for (int i = 1; i < 32; ++i) _mask[i] = 0xFFFFFFFF >> (32 - i);
}
