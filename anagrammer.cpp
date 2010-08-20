#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include "anagrammer.h"
using namespace std;

#define BLANK 26
#define NUM_LETTERS 26
#define NUM_TILES 27

Anagrammer::Anagrammer(char* dict) {
    dawg = (const uint*)loadDawg(dict);
    computeMasks();
}

// convert an index into the tileset into a tile character
inline char Anagrammer::toChar(uint index) {
    if (index == BLANK) return '?';
    return index + 'A';
}

inline void Anagrammer::printLowercase(uchar *perm, int length) {
    for (int i = 0; i < length; ++i) {
        char c = perm[i] - 'A' + 'a';
        cout << c;
    }
    cout << endl;
}

inline void Anagrammer::printTruncated(uchar *permutation, int length) {
    for (int i = 0; i < length; ++i) cout << permutation[i];
    cout << endl;
}

inline bool Anagrammer::hasChild(const uint *dawg, uint nodeIndex, uchar c) {
    uint bits = dawg[nodeIndex];
    uint bit = 1 << (c - 'A');
    return (bits & bit) != 0;
}

inline uint Anagrammer::getChild(const uint *dawg, uint nodeIndex, char c) {
    uint bits = dawg[nodeIndex];
    uint n = c - 'A';
    return dawg[nodeIndex + 1 + __builtin_popcount(bits & mask[n])];
}

inline bool
Anagrammer::terminates(unsigned int node) {
    return (node & 1) != 0;
}

inline unsigned int
Anagrammer::getPointer(unsigned int node) {
    return node >> 8;
}

inline bool Anagrammer::skipAhead(uchar *perm, int start) {
    for (int i = start + 1; perm[i] != '\0'; ++i) {
        if (perm[i] > perm[start]) {
            swap(perm[start], perm[i]);
            return true;
        }
    }
 
    char temp = perm[start];
    int numOfTemp = 1;
    for (uchar* dupPtr = perm + start + 1; dupPtr[0] == temp; ++dupPtr) {
        ++numOfTemp;
    }

    uchar* cyclePtr = perm + start;
    while (cyclePtr[numOfTemp] != '\0') {
        cyclePtr[0] = cyclePtr[numOfTemp];
        ++cyclePtr;
    }
    for (int i = 0; i < numOfTemp; ++i) cyclePtr[i] = temp;
 
    return false;
}

inline int Anagrammer::countTiles(const char *input, uint *counts) {
    uint validChars = 0;
    for (int i = 0; i < NUM_TILES; i++) counts[i] = 0;

    for (const char* p = input; p[0] != '\0'; ++p) {
        char c = p[0];
        int i;
        bool valid = true;

        if ((c >= 'a') && (c <= 'z')) {
            i = c - 'a';
        } else if ((c >= 'A') && (c <= 'Z')) {
            i = c - 'A';
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

void Anagrammer::anagram(const char *input) {
    uint counts[NUM_TILES];
    uint validChars = countTiles(input, counts);
    if (!validChars) {
        cout << "no valid input" << endl;
        return;
    }

    /* This is an implementation of Algorithm L (Lexicographic permutation
       generation) from Knuth's TAOCP Volume 4, Chapter 7.2.1.2. */
    // L1. [Visit a_1,a_2...a_n]
    uchar *perm = new uchar[validChars + 1];
    uint index = 0;
    for (int tile = 0; tile < NUM_TILES; ++tile) {
        for (int j = 0; j < counts[tile]; ++j) {
            perm[index++] = toChar(tile);
        }
    }
    perm[index] = '\0';

    uint *nodes = new unsigned int[validChars];
    nodes[0] = 0;
    uint newestPrefixLen = 1;
    uint skipUntilNewAt = 1;
    for (;;) {
        if (newestPrefixLen <= skipUntilNewAt + 1) {
            for (int i = newestPrefixLen; i <= validChars; ++i) {
                if (hasChild(dawg, nodes[i - 1], perm[i - 1])) {
                    uint child = getChild(dawg, nodes[i - 1], perm[i - 1]);
                    if (terminates(child)) printTruncated(perm, i);
                    unsigned int newNode = getPointer(child);
                    nodes[i] = newNode;
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
        while (perm[j] >= perm[j + 1]) {
            if (j == 0) {
                delete perm;
                delete nodes;
                return;
            }
            --j;
        }

        if (j > skipUntilNewAt) {
            while (!skipAhead(perm, skipUntilNewAt)) {
                if (!skipUntilNewAt) {
                    delete perm;
                    delete nodes;
                    return;
                }
                --skipUntilNewAt;
            }
            newestPrefixLen = skipUntilNewAt + 1;
        } else {
            // L3. [Increase a_j]
            int l = validChars - 1;
            while (perm[j] >= perm[l]) --l;
            swap(perm[j], perm[l]);
            newestPrefixLen = j + 1;

            // L4. [Reverse a_j+1.,.a_n]
            int r0 = j + 1;
            int r1 = validChars - 1;
            while (r0 < r1) swap(perm[r0++], perm[r1--]);
        }
    }
}

const uint* Anagrammer::loadDawg(char *filename) {
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
    mask[0] = 0;
    for (int i = 1; i < 32; ++i) mask[i] = 0xFFFFFFFF >> (32 - i);
}
