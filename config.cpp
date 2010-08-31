/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#include <fstream>
#include <iostream>
#include <string.h>
#include "config.h"
#include "board.h"
using namespace std;

Config::Config(const char* layout, const char *tiles) {
    char layoutFile[100];
    strcpy(layoutFile, layout);
    strcat(layoutFile, ".qly");
    loadLayout(layoutFile);

    char tilesFile[100];
    strcpy(tilesFile, tiles);
    strcat(tilesFile, ".qtd");
    loadTiles(tilesFile);
}

void Config::loadLayout(const char *filename) {
    _valid = true;

    string by; // dummy
    ifstream file(filename);
    file >> _width;
    file >> by;
    file >> _height;
    if (!checkDimensions()) goto invalid;
    if (!readGrid(file)) goto invalid;
    _startRow = _height / 2;
    _startCol = _width / 2;
    return;

 invalid:
    _valid = false;
}

void Config::loadTiles(const char *filename) {
    // read in .qtd file with tile distribution and scores
    ifstream file(filename);
    for (uint i = 0; i <= BLANK; ++i) _counts[i] = 0;
    for (uint i = 0; i <= BLANK; ++i) _scores[i] = 0;
    for (;;) {
        uchar c;
        file >> c;

        uint score;
        file >> score;

        uint count;
        file >> count;
        
        if (file.eof()) break;
        _counts[Util::charToIndex(c)] = count;
        _scores[Util::charToIndex(c)] = score;
        //cout << c << " " << score << " " << count << endl;
    }
}

bool Config::checkDimensions() {
    if (_width < MINIMUM_BOARD_SIZE) {
        cout << "width is smaller than minimum of "
             << MINIMUM_BOARD_SIZE << endl;
        return false;
    }
    if (_height < MINIMUM_BOARD_SIZE) {
        cout << "height is smaller than minimum of "
             << MINIMUM_BOARD_SIZE << endl;
        return false;
    }
    return true;
}

bool Config::readGrid(ifstream &file) {
    for (int row = 0; row < _height; ++row) {
        for (int col = 0; col < _width; ++col) {
            string square;
            file >> square;
            //cout << "square: " << square << endl;
            if (square.length() != 2) {
                cout << square << " is invalid square" << endl;
                return false;
            }
            if (square == "..") {
                _lMul[row][col] = 1;
                _wMul[row][col] = 1;
            } else {
                int mul;
                if ((square[0] >= '1') && (square[0] <= '4')) {
                    mul = square[0] - '0';
                } else {
                    cout << square[0] << " is invalid multiplier" << endl;
                    return false;
                }
                switch (square[1]) {
                case 'L':
                    _lMul[row][col] = mul;
                    _wMul[row][col] = 1;
                    break;
                case 'W':
                    _lMul[row][col] = 1;
                    _wMul[row][col] = mul;
                    break;
                default:
                    cout << square[1] << " is invalid multiplier type" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}
            
void Config::writeSquare(ostream &o, int row, int col) const {
    char square = ' ';
    if (_lMul[row][col] > 1) {
        switch(_lMul[row][col]) {
        case 2: square = '\''; break;
        case 3: square = '"'; break;
        case 4: square = '^'; break;
        }
    } else {
        switch(_wMul[row][col]) {
        case 2: square = '-'; break;
        case 3: square = '='; break;
        case 4: square = '#'; break;
        }
    }
    o << square;
}
