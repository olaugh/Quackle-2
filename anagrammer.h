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

#include <cstdint>
#include <map>
#include <tr1/unordered_map>
#include <vector>
#include "board.h"
#include "constants.h"
#include "move.h"
#include "types.h"
#include "rack.h"
#include "util.h"
using namespace std;

class Anagrammer
{
 public:
    Anagrammer(const char* lex);
    void anagram(const char* input);
    void findMoves(const Board &board, const Rack &rack);
    vector<Move>* moves() { return &_moves; }
    inline bool isValid() const { return _valid; }
    static void convertLeaves(const char *input, const char *output);
    void testLeaves();
    float prodValue(uint64_t product);
    uint64_t stringProd(const char *s);

 private:
    struct leave_t {
        uint64_t product;
        float value;
    };
        
    uint _mask[32];
    const uint *_dawg;
    const uchar *_leaves;
    const uint64_t *_primes;
    uint _numLeaves;

    uchar _perm[MAXIMUM_RACK_SIZE + 1];
    uint _nodes[MAXIMUM_RACK_SIZE + 1];
    bool _valid;
    uint _rackLen;
    uint _counts[BLANK + 1];
    uint64_t _rackProd;
    uint _rackScores[MAXIMUM_RACK_SIZE];

    vector<Move> _moves;

    struct Square {
        uint row;
        uint col;
        uint horiz;
        uint minLen;
        uint maxLen;
        uint node;
    };

    vector<Square> _squares;

    inline char toChar(unsigned int index);
    inline void printTruncated(int length);
    inline bool hasChild(uint nodeIndex, uchar c);
    inline unsigned int getChild(uint nodeIndex, char c);
    inline bool terminates(uint node);
    inline unsigned int getPointer(uint node);

    inline leave_t* indexLeave(uint index);

    inline bool skipAhead(uint start);
    inline void increase(uint pos);
    inline void reverseAfter(uint pos);

    inline uint countTiles(const char *input);
    inline void countRackTiles(const Rack &rack);
    inline uint setFirstPerm(const char *input);
    inline void setRackFirstPerm(const Rack &rack);
    inline void lookupRackScores(const Config *config, const Rack &r);
    inline void findSquares(const Board &board, const Rack &rack);
    inline void findOpenerSquares(const Board &board, const Rack &rack);
    inline void findNonOpenerSquares(const Board &board, const Rack &rack);
    inline void findScoringPlays(const Board &Board, const Rack &rack);
    inline void findExchanges(const Rack &rack);
    inline void findPass();

    const uint* loadDawg(const char *filename);
    const uchar* loadLeaves(const char *filename);
    const uint64_t* loadPrimes(const char *filename);

    void computeMasks();
};

#endif
