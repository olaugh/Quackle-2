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
#include "constants.h"
#include "layout.h"
using namespace std;

class Test
{
 public:
    static void testAnagram(const char *input, int times);
    static void testBoard();
    static void testMove();
    static void testRack();
    static void testOpener();
};

#endif
