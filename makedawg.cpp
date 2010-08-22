/*************************************************************
 *
 * Copyright (C) 2010 John O'Laughlin
 *
 * All rights reserved.
 *
 *************************************************************
 */

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;

class Node {
public:
    char c;
    bool t;
    vector<Node> children;
    unsigned int pointer;
    void pushword(string word);
    void print(string prefix);
    void write(ofstream& out);
};

unsigned int nodeIndex;

void Node::write(ofstream& out) {
    //cout << "node.pointer: " << pointer << endl;

    //cout << "children: ";
    unsigned int bits = 0;
    for (int i = 0; i < children.size(); ++i) {
        //cout << children[i].c;
        unsigned int b = children[i].c - 'A';
        bits |= (1 << b); 
    }
    //cout << endl;
    const unsigned char bitBytes[4] = {
        (bits & 0x000000FF) >>  0,
        (bits & 0x0000FF00) >>  8,
        (bits & 0x00FF0000) >> 16,
        (bits & 0xFF000000) >> 24
    };
    out.write((const char*)bitBytes, 4);

    for (int i = 0; i < children.size(); ++i) {
        unsigned int p = children[i].pointer;
        //cout << "child " << children[i].c << " has index " << p << endl;
        const unsigned char childBytes[4] = {
            children[i].t ? 1 : 0,
            (p & 0x000000FF) >>  0,
            (p & 0x0000FF00) >>  8,
            (p & 0x00FF0000) >> 16
        };
        out.write((const char*)childBytes, 4);
    }
    
    for (int i = 0; i < children.size(); ++i) {
        if (children[i].children.size() > 0) 
            children[i].write(out);
    }
}

void Node::print(string prefix) {
    if (t) {
        //cout << prefix << endl;
    }

    pointer = nodeIndex;
    //cout << "prefix " << prefix << " is given index " << nodeIndex << endl;
    // one "node" space will be taken up by the bitvector of child-letters
    nodeIndex += 1 + children.size();

    /*
    cout << "prefix " << prefix << " can be extended with any of: ";
    for (int i = 0; i < children.size(); i++) {
        cout << children[i].c;
    }
    cout << endl;
    */

    for (int i = 0; i < children.size(); i++) {
        if (children[i].children.size() > 0) {
            children[i].print(prefix + children[i].c);
        }
    }
}

void Node::pushword(string word) {
    if (word.length() == 0) {
        t = true;
    }
    else {
        char first = word[0];
        string rest = word.substr(1, word.length() - 1);
        int index = -1;
 
        // cout << "first: " << first << ", rest: " << rest << endl;

        for (int i = 0; i < children.size(); i++) {
            if (children[i].c == first) {
                index = i;
                i = children.size();
            }
        }
        
        if (index == -1) {
            Node n;
            n.c = first;
            n.t = false;
            n.pointer = 0;
            children.push_back(n);
            index = children.size() - 1;
        }

        children[index].pushword(rest);
    }
}


int main() {
    Node root;
    root.t = false;
    root.c = '.';
    root.pointer = 0;

    ifstream file("twl.txt");
    while(!file.eof()) {
        string word;
        file >> word;
        if (!file.eof()) root.pushword(word);
    }

    nodeIndex = 0;
    root.print("");
    cout << "number of nodes: " << nodeIndex << endl;

    ofstream out("output.dawg", ios::out | ios::binary);
    out.write("DAWG", 4);

    const char sizeBytes[] = {
        (nodeIndex & 0x000000FF) >>  0,
        (nodeIndex & 0x0000FF00) >>  8,
        (nodeIndex & 0x00FF0000) >> 16,
        (nodeIndex & 0xFF000000) >> 24
    };
    out.write(sizeBytes, 4);
    root.write(out);
}
