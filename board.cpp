
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "board.h"

using namespace std;

Board::Board(int width, int height)
  : m_width(width), m_height(height), m_empty(true) {
}

inline bool
Board::isEmpty() {
    return m_empty;
}

ostream &operator<<(ostream &o, const Board &board) {
    o << "<Board>" << endl;
}
