/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#include "test.h"
using namespace std;

void Test::testAnagram(char *input, int times) {
    Anagrammer a((char *)"twl.dawg");
    for (int i = 0; i < times; ++i) a.anagram(input);
}

void Test::testBoard() {
    Layout standard((char*)"standard.qly");
    Board b(15, 15, &standard);
    cout << b;
}

void Test::testMove() {
    Move pass;
    uchar q[1] = {16};
    Move exch(1, q);
    uchar placeLetters[5] = {15, 11, 0, 2, 4};
    uchar placeTiles[5] = {15, 11, BLANK, 2, 4};
    Move place(5, placeTiles, placeLetters, 7, 3, true);
    cout << pass << endl;
    cout << exch << endl;
    cout << place << endl;
}
