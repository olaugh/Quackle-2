#ifndef BOARD_H
#define BOARD_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#define MAXIMUM_BOARD_SIZE 25

class Board
{
 public:
    Board(int width, int height);

    bool isEmpty();

 private:
    int m_width;
    int m_height;
    bool m_empty;

    char m_letters[MAXIMUM_BOARD_SIZE][MAXIMUM_BOARD_SIZE];

};

ostream &operator<<(ostream &o, const Board &board);
#endif
