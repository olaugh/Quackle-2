#ifndef TEST_H
#define TEST_H

#include <iostream>
#include "anagrammer.h"
#include "board.h"
#include "layout.h"
using namespace std;

class Test
{
 public:
    static void testAnagram(char *input, int times);
    static void testBoard();
};

#endif

