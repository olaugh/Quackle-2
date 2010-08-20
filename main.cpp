#include <iostream>

#include "anagrammer.h"
#include "board.h"

using namespace std;

int main(int argc, char **argv) {
    if (argc <= 1) {
        cout << "usage: " << argv[0] << " foo" << endl;
        cout << "(will build subwords of foo)" << endl;
    } else {
        Anagrammer a((char*)"twl.dawg");
        Board b(15, 15);
        for (int i = 0; i < 1; ++i) {
            a.anagram(argv[1]);
        }
    }
    return 0;
}
