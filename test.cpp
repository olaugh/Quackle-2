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

void Test::testAnagram(const char *input, int times) {
    Anagrammer a("twl.dawg");
    for (int i = 0; i < times; ++i) a.anagram(input);
}

void Test::testBoard() {
    Layout standard("standard.qly");
    Board b(15, 15, &standard);
    cout << b;
}

void Test::testMove() {
    Move pass;
    uchar q[1] = {17};
    Move exch(1, q);
    uchar placeLetters[5] = {16, 12, 1, 3, 5};
    uchar placeTiles[5] = {16, 12, BLANK, 3, 5};
    Move place(5, placeTiles, placeLetters, 7, 3, true);
    cout << pass << endl;
    cout << exch << endl;
    cout << place << endl;
}

void Test::testRack() {
    uchar rucksexTiles[7] = {18, 21, 3, 11, 19, 5, 24};
    Rack rucksex(7, rucksexTiles);
    cout << rucksex << endl;
}

void Test::testOpener() {
    Layout standard("standard.qly");
    Board empty(15, 15, &standard);
    uchar rucksexTiles[7] = {18, 21, 3, 11, 19, 5, 24};
    Rack rucksex(7, rucksexTiles);
    Anagrammer a("twl.dawg");
    a.findMoves(empty, rucksex);
}
