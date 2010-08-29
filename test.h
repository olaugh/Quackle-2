/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#ifndef TEST_H
#define TEST_H

#include <iostream>
#include "anagrammer.h"
#include "board.h"
#include "config.h"
#include "constants.h"
#include "types.h"
using namespace std;

class Test
{
 public:
    static void testAnagram(const char *input, int times);
    static void testBoard();
    static void testMove();
    static void testRack();
    static void testOpener();
    static void testConvertLeaves();
    static void testLeaves();
    static void testLeaveLookup();
};

#endif
