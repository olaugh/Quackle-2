#include <iostream>
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
