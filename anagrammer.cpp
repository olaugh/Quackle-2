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
#include <string.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include "anagrammer.h"
using namespace std;

Anagrammer::Anagrammer(const char* lex) {
    _valid = true;

    char dict[100];
    strcpy(dict, lex);
    strcat(dict, ".dawg");
    _dawg = loadDawg(dict);

    char leaves[100];
    strcpy(leaves, lex);
    strcat(leaves, ".qsl");
    _leaves = loadLeaves(leaves);

    char primes[100];
    strcpy(primes, lex);
    strcat(primes, ".qpr");
    _primes = loadPrimes(primes);

    computeMasks();
}

inline void Anagrammer::printTruncated(int length) {
    for (int i = 0; i < length; ++i) Util::writeTile(cout, _perm[i]);
    cout << endl;
}

inline void Anagrammer::printTruncated(int length, bool octo) {
    for (int i = 0; i < length; ++i) Util::writeTile(cout, _perm[i]);
    if (octo) cout << "#";
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

inline uchar Anagrammer::getChildLetter(uint nodeIndex, uint idx) {
    uint setBits = 0;
    uint bits = _dawg[nodeIndex];
    for (uint letter = FIRST_LETTER; letter < BLANK; ++letter) {
        uint bit = 1 << letter;
        if (bits & bit) ++setBits;
        if (setBits == idx + 1) return letter;
    }
}

inline void Anagrammer::printChildren(uint nodeIndex) {
    uint bits = _dawg[nodeIndex];
    for (uint letter = FIRST_LETTER; letter < BLANK; ++letter) {
        uint bit = 1 << letter;
        if (bits & bit) Util::writeLetter(cout, letter);
    }
    cout << endl;
}

inline uint Anagrammer::numChildren(uint nodeIndex) {
    return __builtin_popcount(_dawg[nodeIndex]);
}

inline bool Anagrammer::terminates(unsigned int node) {
    return (node & 1) != 0;
}

inline bool Anagrammer::inSmallerDict(unsigned int node) {
    return (node & 2) != 0;
}

inline unsigned int Anagrammer::getPointer(unsigned int node) {
    return node >> 8;
}

inline bool Anagrammer::skipAhead(uint start) {
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

inline uint Anagrammer::countTiles(const char *input) {
    _rackProd = 1;
    for (int i = FIRST_LETTER; i <= BLANK; ++i) _counts[i] = 0;
    uint validChars = 0;
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
            ++_counts[i];
            _rackProd *= _primes[i];
            ++validChars;
        }
    }
    return validChars;
}

inline void Anagrammer::countRackTiles(const Rack &rack) {
    _rackProd = 1;
    for (int i = FIRST_LETTER; i <= BLANK; i++) _counts[i] = 0;
    for (int i = 0; i < _rackLen; ++i) {
        ++_counts[rack.tile(i)];
        _rackProd *= _primes[rack.tile(i)];
    }
}

inline uint Anagrammer::setFirstPerm(const char *input) {
    uint validChars = countTiles(input);
    if (!validChars) {
        cout << "no valid input" << endl;
        return 0;
    }

    /* This is an implementation of Algorithm L (Lexicographic permutation
       generation) from Knuth's TAOCP Volume 4, Chapter 7.2.1.2. */
    // L1. [Visit a_1,a_2...a_n]
    uint index = 0;
    for (int tile = FIRST_LETTER; tile <= BLANK; ++tile)
        for (int j = 0; j < _counts[tile]; ++j) _perm[index++] = tile;
    _perm[index] = '\0';

    return validChars;
}

inline void Anagrammer::setRackFirstPerm(const Rack &rack) {
    countRackTiles(rack);
    /* This is an implementation of Algorithm L (Lexicographic permutation
       generation) from Knuth's TAOCP Volume 4, Chapter 7.2.1.2. */
    // L1. [Visit a_1,a_2...a_n]
    uint index = 0;
    for (int tile = FIRST_LETTER; tile <= BLANK; ++tile)
        for (int j = 0; j < _counts[tile]; ++j) _perm[index++] = tile;
    _perm[index] = '\0';
}

inline void
Anagrammer::findOpenerSquares(const Board &board, const Rack &rack) {
    _squares.clear();
    uint row = board.config()->startRow();
    uint startCol = board.config()->startCol();
    uint len = rack.len();
    for (uint col = startCol - len + 1; col <= startCol; ++col) {
        Square sq;
        sq.row = row;
        sq.col = col;
        sq.horiz = true;
        sq.minLen = std::max(2, (int)(startCol) - (int)(col) + 1);
        sq.maxLen = len;
        _squares.push_back(sq);
    }
}

inline void
Anagrammer::findNonOpenerSquares(const Board &board, const Rack &rack) {
    _squares.clear();
}

inline void Anagrammer::findSquares(const Board &board, const Rack &rack) {
    if (board.isEmpty()) findOpenerSquares(board, rack);
    else findNonOpenerSquares(board, rack);
    
    /*
    vector<Square>::iterator end(_squares.end());
    for (vector<Square>::iterator it = _squares.begin(); it != end; ++it) {
        cout << "sq: " << it->row << " " << it->col
             << " min: " << it->minLen << " max: " << it->maxLen << endl;
    }
    */
}

inline void Anagrammer::lookupRackScores(const Config *config, const Rack &r) {
    for (int i = 0; i < _rackLen; ++i)
        _rackScores[i] = config->tileScore(_perm[i]);
    //for (int i = 0; i < _rackLen; ++i) cout << _rackScores[i] << " ";
    //cout << endl;
}

void Anagrammer::findMoves(const Board &board, const Rack &rack) {
    _moves.clear();
    findSquares(board, rack);
    _rackLen = rack.len();
    setRackFirstPerm(rack);
    lookupRackScores(board.config(), rack);

    findScoringPlays(board, rack);
    findExchanges(rack);
    findPass();
} 

inline void Anagrammer::findPass() {
    Move m;
    _moves.push_back(m);
}

inline void Anagrammer::findExchanges(const Rack &rack) {
    uint counts[BLANK + 1];
    uchar uniqTiles[MAXIMUM_RACK_SIZE + 1];
    uint toTrade[MAXIMUM_RACK_SIZE + 1];
    uint numUniq = 0;
    for (uint i = FIRST_LETTER; i <= BLANK; ++i) {
        if (_counts[i]) {
            uniqTiles[numUniq] = i;
            counts[numUniq++] = _counts[i];
        }
    }

    toTrade[0] = 1;
    for (uint i = 1; i < numUniq + 1; ++i) toTrade[i] = 0;
    for (;;) {
        uchar tiles[MAXIMUM_RACK_SIZE];
        uint len = 0;
        uint64_t prod = 1;
        for (uint i = 0; i < numUniq; ++i)
            for (uint j = 0; j < toTrade[i]; ++j) {
                tiles[len++] = uniqTiles[i];
                prod *= _primes[uniqTiles[i]];
            }
        Move m(len, tiles, prodValue(_rackProd / prod));
        _moves.push_back(m);
        
        uint place;
        for (place = 0; place < numUniq; ++place)
            if (toTrade[place] < counts[place]) {
                ++toTrade[place];
                break;
            } else toTrade[place] = 0;
        if (place == numUniq) return;
    }
}


inline void Anagrammer::findScoringPlays(const Board &board, const Rack &rack) {
    _nodes[0] = 0;
    uint activeBlanks = 0;
    BlankState bStates[MAXIMUM_RACK_SIZE];
    BlankState *bState;
    uint newestPrefixLen = 1;
    uint skipUntilNewAt = 1;
    const Config *conf = board.config();
    for (;;) {
        uint pos = newestPrefixLen - 1;
        uchar tile = _perm[pos];
        uchar letter;
        cout << "pos: " << pos << endl;
        cout << "_perm: ";
        printTruncated(7);
        //cout << "letter: ";
        //Util::writeTile(cout, tile);
        //cout << endl;

        if (newestPrefixLen <= skipUntilNewAt + 1) {
            for (; pos <= _rackLen; ++pos) {
                tile = _perm[pos];
                letter = tile;

                //cout << "  checking prefix ";
                //printTruncated(pos);

                if (tile == BLANK) {
                    bState = &bStates[activeBlanks++];
                    bState->idx = 0;
                    bState->lim = numChildren(_nodes[pos]);
                    bState->pos = pos;
                increment_blank:
                    cout << "activeBlanks: " << activeBlanks << endl;
                    cout << "  bState->idx: " << bState->idx << endl;
                    cout << "  bState->lim: " << bState->lim << endl;
                    pos = bState->pos;
                    cout << "  node's child letters: ";
                    printChildren(_nodes[pos]);
                    letter = getChildLetter(_nodes[pos], bState->idx);
                    bState->ltr = letter;
                    cout << "  blankLetter: ";
                    Util::writeTile(cout, letter);
                    cout << endl;
                    //cout << "  _perm: ";
                    //printTruncated(newestPrefixLen);
                    //printTruncated(7);
                } 

                //uchar letter = activeBlanks ? bState->ltr : _perm[i - 1];
                int len = pos + 1;
                printTruncated(pos);
                cout << "  has child letter ";
                Util::writeTile(cout, letter);
                cout << "?" << endl;
                if (hasChild(_nodes[pos], letter)) {
                    //cout << "    yes" << endl;
                    uint child = getChild(_nodes[pos], letter);
                    uint64_t prod = 1;
                    for (uint j = 0; j <= pos; ++j) prod *= _primes[_perm[j]];
                    if (terminates(child)) {
                        vector<Square>::iterator end(_squares.end());
                        float leave = prodValue(_rackProd / prod);
                        //cout << (_rackProd / prod) << " " << leave << endl;
                        int bonus = (len == _rackLen) ? 50 : 0;
                        for (vector<Square>::iterator it = _squares.begin();
                             it != end; ++it) {
                            int score = 0;
                            uint wMul = 1;
                            if ((len >= it->minLen) && (len <= it->maxLen)) {
                                for (uint j = 0; j < len; ++j) {
                                    uint col = it->col + j;
                                    uint tScr = conf->tileScore(_perm[j]);
                                    score += tScr * conf->lMul(it->row, col);
                                    wMul *= conf->wMul(it->row, col);
                                }
                                score = score * wMul + bonus;
                                uchar letters[MAXIMUM_RACK_SIZE + 1];
                                for (int i = 0; i < len; ++i)
                                    letters[i] = _perm[i];
                                for (int i = 0; i < activeBlanks; ++i)
                                    letters[bStates[i].pos] = bStates[i].ltr;
                                Move m(len, _perm, letters, it->row, it->col,
                                       true, score, score + leave);
                                cout << "pushing move: " << m << endl;
                                _moves.push_back(m);
                            }
                        }
                    }
                    unsigned int newNode = getPointer(child);
                    _nodes[pos + 1] = newNode;
                    if (!newNode) {
                        skipUntilNewAt = pos;
                        break;
                    }
                } else {
                    //cout << "    no" << endl;
                    skipUntilNewAt = pos;
                    break;
                }
            }
        }

        // L2. [Find j]
        int j = _rackLen - 2;
        while (_perm[j] >= _perm[j + 1]) {
            if (j-- == 0) {
                if (activeBlanks && (++bState->idx < bState->lim)) {
                    cout << "  incrementing blank" << endl;
                    newestPrefixLen = bState->pos + 1;
                    goto increment_blank;
                }
                return;
            }
        }

        if (j > skipUntilNewAt) {
            cout << "no words with prefix ";
            printTruncated(skipUntilNewAt + 1);

            if (activeBlanks && (skipUntilNewAt <= bState->pos)
                             && (++bState->idx < bState->lim)) {
                cout << "  incrementing blank" << endl;
                newestPrefixLen = bState->pos + 1;
                goto increment_blank;
            }

            while (!skipAhead(skipUntilNewAt)) {
                if (!(skipUntilNewAt--)) {
                     if (activeBlanks && (++bState->idx < bState->lim)) {
                         uchar temp = _perm[_rackLen - 1];
                         for (int i = _rackLen - 2; i >= 0; --i)
                             _perm[i + 1] = _perm[i];
                         _perm[0] = temp;
                         cout << "  incrementing blank" << endl;
                         newestPrefixLen = bState->pos + 1;
                         goto increment_blank;
                     }
                    return;
                }
            }

            newestPrefixLen = skipUntilNewAt + 1;
            cout << "  skipped ahead, newest prefix len now "
                 << newestPrefixLen << endl;

            if (activeBlanks) {
                cout << "  are we really skipping? a blank is active" << endl;
                cout << "activeBlanks: " << activeBlanks << endl;
                cout << "skipUntilNewAt: " << skipUntilNewAt << endl;
                cout << "bState->pos: " << bState->pos << endl;
                if (++bState->idx < bState->lim) {
                    cout << "  _perm: ";
                    printTruncated(7);
                    if (_perm[bState->pos] != BLANK) {
                        cout << "   blank is out of its position!" << endl;
                        for (uint i = newestPrefixLen; i < _rackLen; ++i) {
                            if (_perm[i] == BLANK) {
                                std::swap(_perm[i], _perm[bState->pos]);
                                break;
                            }
                        }
                    }
                    sort(_perm + bState->pos + 1, _perm + _rackLen);
                    cout << "  (sorted?) _perm: ";
                    printTruncated(7);
                    cout << "  incrementing blank" << endl;
                    newestPrefixLen = bState->pos + 1;
                    goto increment_blank;
                }
                if (skipUntilNewAt <= bState->pos) {
                    cout << "    deleting blank" << endl;
                    --activeBlanks;
                }
            }
        } else {
	    // if permutation says to swap a letter before a blank, instead
	    // advance the blank and reset to its first permutation
            if (activeBlanks) {
                bState = &bStates[activeBlanks - 1];
                if (j < bState->pos) {
                    cout << "  moving over blank (" << j << " < "
                         << bState->pos << ")" << endl;
                    if (++bState->idx < bState->lim) {
                        cout << "    advancing blank" << endl;
                        newestPrefixLen = bState->pos + 1;
                        goto increment_blank;
                    } else {
                        cout << "    deleting blank" << endl;
                        --activeBlanks;
                    }
                }
            }
            // L3. [Increase a_j]
            increase(j);
            newestPrefixLen = j + 1;
            // L4. [Reverse a_j+1.,.a_n]
            reverseAfter(j);
        }
    }
}

inline void Anagrammer::increase(uint pos) {
    int l = _rackLen - 1;
    while (_perm[pos] >= _perm[l]) --l;
    swap(_perm[pos], _perm[l]);
}

inline void Anagrammer::reverseAfter(uint pos) {
    int r0 = pos + 1;
    int r1 = _rackLen - 1;
    while (r0 < r1) swap(_perm[r0++], _perm[r1--]);
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
                    if (terminates(child)) {
                        printTruncated(i, !inSmallerDict(child));
                    }
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
    if (strcmp(fileType, "DAWG")) {
        cout << filename << " is not a valid DAWG file" << endl;
        return NULL;
    }

    uint dawgNodes;
    file.read((char*)&dawgNodes, 4);
    
    char *dawg = new char[dawgNodes * 4];
    file.read(dawg, dawgNodes * 4);
    return (uint*)dawg;
}

const uchar* Anagrammer::loadLeaves(const char *filename) {
    ifstream file(filename);
    
    file.read((char*)&_numLeaves, 4);
    
    char *leaves = new char[_numLeaves * 12];
    file.read(leaves, _numLeaves * 12);
    return (uchar*)leaves;
}

const uint64_t* Anagrammer::loadPrimes(const char *filename) {
    ifstream file(filename);
    uint64_t *primes = new uint64_t[BLANK + 1];
    for (uint i = 0; i <= BLANK; ++i) primes[i] = 0;
    for (;;) {
        uchar c;
        file >> c;

        uint64_t p;
        file >> p;

        if (file.eof()) break;
        primes[Util::charToIndex(c)] = p;
    }
    return primes;
}

float Anagrammer::prodValue(uint64_t product) {
    if (product == 1) return 0.0f;
    uint min = 0;
    uint max = _numLeaves - 1;
    uint mid;
    uint64_t midProduct;

    do {
        mid = min + (max - min) / 2;
        midProduct = indexLeave(mid)->product;
        if (product > midProduct) min = mid + 1;
        else max = mid - 1;
    } while ((midProduct != product) && (min <= max));

    if (midProduct == product) return indexLeave(mid)->value;
    else {
        cout << "could not find value for product " << product << "!" << endl;
        return 0.0f;
    }
}

inline Anagrammer::leave_t* Anagrammer::indexLeave(uint index) {
    return (leave_t*)(_leaves + 12 * index);
}

uint64_t Anagrammer::stringProd(const char *s) {
    uint64_t prod = 1;
    for (uint i = 0; s[i]; ++i) prod *= _primes[Util::charToIndex(s[i])];
    return prod;
}

void Anagrammer::testLeaves() {
    for (int i = 0; i < _numLeaves; ++i) {
        leave_t *leave = (leave_t*)(_leaves + 12 * i);
        cout << leave->product << " " << leave->value << endl;
    }
}

void Anagrammer::convertLeaves(const char *input, const char *output) {
    ifstream in(input);
    ofstream out(output, ios::out | ios::binary);
    uint numEntries;
    in >> numEntries;
    out.write((char*)&numEntries, sizeof(uint));
    while (!in.eof()) {
        uint64_t product;
        float value;
        in >> product;
        in >> value;
        //cout << product << " " << value << endl;
        out.write((char*)&product, sizeof(uint64_t));
        out.write((char*)&value, sizeof(float));
    }
    in.close();
    out.close();
}

void Anagrammer::computeMasks() {
    _mask[0] = 0;
    for (int i = 1; i < 32; ++i) _mask[i] = 0xFFFFFFFF >> (32 - i);
}
