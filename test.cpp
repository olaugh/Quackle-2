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
    Anagrammer a("twl");
    if (!a.isValid()) return;
    for (int i = 0; i < times; ++i) a.anagram(input);
}

void Test::testBoard() {
    Config standard("standard", "english");
    Board b(15, 15, &standard);
    cout << b;
}

void Test::testMove() {
    Move pass;
    uchar q[1] = {17};
    Move exch(1, q, 0.0f);
    uchar placeLetters[5] = {16, 12, 1, 3, 5};
    uchar placeTiles[5] = {16, 12, BLANK, 3, 5};
    Move place(5, placeTiles, placeLetters, 7, 3, true, 22, 32.0f);
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
    Config standard("standard", "english");
    Board empty(15, 15, &standard);
    uchar rucksexTiles[7] = {18, 21, 3, 11, 19, 5, 24}; //rucksex
    //uchar rucksexTiles[7] = {18, 5, 20, 1, 9, 14, 19};  //retains
    //uchar rucksexTiles[7] = {22, 9, 22, 9, 6, 9, 3}; //vivific
    Rack rucksex(7, rucksexTiles);
    Anagrammer a("twl");
    if (!a.isValid()) return;
    for (int i = 0; i < 1; ++i) {
        a.findMoves(empty, rucksex);
        vector<Move> *moves = a.moves();
        Move::sortByEquity(moves);
        vector<Move>::iterator end(moves->end());
        for (vector<Move>::iterator it = moves->begin(); it != end; ++it) {
            cout << *it << " (" << it->score() << " points) "
                 << it->equity() << endl;
        }
    }
}

void Test::testConvertLeaves() {
    Anagrammer::convertLeaves("products.txt", "twl.qsl");
}
    
void Test::testLeaves() {
    Anagrammer a("twl");
    a.testLeaves();
}

void Test::testLeaveLookup() {
    Anagrammer a("twl");

    uint64_t emptyProd = 1;
    float emptyValue = a.prodValue(emptyProd);
    cout << "leave value for product " << emptyProd << " is " << emptyValue << endl;

    uint64_t prod = 187302;
    float value = a.prodValue(prod);
    cout << "leave value for product " << prod << " is " << value << endl;

    uint64_t badProd = 187303;
    float badValue = a.prodValue(badProd);
    cout << "leave value for product " << badProd << " is " << badValue << endl;

    const char *satine = "SATINE";
    uint64_t satineProd = a.stringProd(satine);
    float satineValue = a.prodValue(satineProd);
    cout << "product for " << satine << " is " << satineProd << endl;
    cout << "value for " << satine << " is " << satineValue << endl;

    const char *jqxz__ = "JQXZ??";
    uint64_t jqxz__Prod = a.stringProd(jqxz__);
    float jqxz__Value = a.prodValue(jqxz__Prod);
    cout << "product for " << jqxz__ << " is " << jqxz__Prod << endl;
    cout << "value for " << jqxz__ << " is " << jqxz__Value << endl;
}
