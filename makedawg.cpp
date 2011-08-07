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
#include <set>

using namespace std;

class Node {
public:
    char c;
    bool t;
    bool inSmaller;
    vector<Node*> children;
    unsigned int pointer;
    unsigned int hash(int depth);
    bool equals(Node &n, int depth);
    string prefix;
    bool deleted;
    Node* cloneOf;
    void pushword(string prefix, string word, bool inSmallerDict);
    void print(string prefix);
    void write(ofstream& out);
};

unsigned int nodeIndex;
Node root;
vector<Node*> nodes;
set<string> smallerDict;

void Node::write(ofstream& out) {
    //cout << "node.pointer: " << pointer << endl;

    //cout << "children: ";
    unsigned int bits = 0;
    for (int i = 0; i < children.size(); ++i) {
        //cout << children[i].c;
        unsigned int b = children[i]->c + 1 - 'A';
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
        unsigned int p = children[i]->deleted 
            ? children[i]->cloneOf->pointer
            : children[i]->pointer;

        //cout << "child " << children[i].c << " has index " << p << endl;
        unsigned char flags =
            (children[i]->t ? 1 : 0) |
            (children[i]->inSmaller ? 2 : 0);

        const unsigned char childBytes[4] = {
            flags,
            (p & 0x000000FF) >> 0,
            (p & 0x0000FF00) >> 8,
            (p & 0x00FF0000) >> 16
        };
        out.write((const char*)childBytes, 4);
    }
    
    for (int i = 0; i < children.size(); ++i) {
        if (children[i]->children.size() > 0) 
            if (!children[i]->deleted) {
                children[i]->write(out);
            }
    }
}

/*
void Node::number() {
    pointer = index;
    index += 1;               // 32 bits for child bits
    index += children.size(); // 32 bits for each child
    for (int i = 0; i < children.size(); ++i) {
        if (children[i].children.size() > 0) {
            if (!children[i].deleted) {
                children[i].number();
            }
        }
    }
}
*/

void Node::print(string prefix) {
    if (t) {
        string maybeOcto = inSmaller ? "" : "#";
        cout << prefix << maybeOcto << endl;
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
        if (children[i]->children.size() > 0) {
            if (children[i]->deleted) {
                cout << prefix + children[i]->c << " is a clone of "
                     << children[i]->cloneOf->prefix << endl;
            } else {
                children[i]->print(prefix + children[i]->c);
            }
        } else if (children[i]->t) {
            string maybeOcto = inSmaller ? "" : "#";
            cout << prefix + children[i]->c << maybeOcto << endl;
        }
    }
}

void Node::pushword(string prefix, string word, bool inSmallerDict) {
    if (word.length() == 0) {
        t = true;
        inSmaller = inSmallerDict;
    }
    else {
        char first = word[0];
        string rest = word.substr(1, word.length() - 1);
        int index = -1;
 
        // cout << "first: " << first << ", rest: " << rest << endl;

        for (int i = 0; i < children.size(); i++) {
            if (children[i]->c == first) {
                index = i;
                i = children.size();
            }
        }
        
        if (index == -1) {
            Node* n = new Node;
            n->c = first;
            n->t = false;
            n->inSmaller = false;
            n->pointer = 0;
            n->prefix = prefix + first;
            n->deleted = false;
            n->cloneOf = NULL;
            children.push_back(n);
            nodes.push_back(n);
            index = children.size() - 1;
        }

        children[index]->pushword(prefix + first, rest, inSmallerDict);
    }
}

void minimize() {
    vector<int> hashed[1023];
    cout << "hashing.." << endl;
    for (int i = 0; i < nodes.size(); i++) {
        //cout << "i: " << i << endl;
        unsigned int modHash = nodes[i]->hash(0) % 1023;
        //cout << "modHash: " << modHash << endl;
        hashed[modHash].push_back(i);
        //cout << "pushed" << endl;
    }
    cout << "finished hashing" << endl;

    for (int i = 0; i < 1023; ++i) {
        for (int j = 0; j < hashed[i].size(); ++j) {
            if (!nodes[hashed[i][j]]->deleted) {
                for (int k = j + 1; k < hashed[i].size(); ++k) {
                    if (nodes[hashed[i][j]]->equals(*nodes[hashed[i][k]], 0)) {
                        /*
                        cout << "minimizing: " << hashed[i][j]
                             << " (" << nodes[hashed[i][j]]->pointer << ")"
                             << "==" << hashed[i][k]
                             << " (" << nodes[hashed[i][k]]->pointer << ")"
                             << endl;
                        */
                        nodes[hashed[i][k]]->deleted = true;
                        nodes[hashed[i][k]]->cloneOf = nodes[hashed[i][j]];
                    }
                }
            }
        }
    }
}

unsigned int Node::hash(int depth) {
    unsigned int bits = 0;
    for (int i = 0; i < children.size(); ++i) {
        unsigned int b = children[i]->c + 1 - 'A';
        bits |= (1 << b);
    }
    
    if (depth > 0) {
        bits += c;
        bits += t ? 2 : 1;
        bits += inSmaller ? 8 : 4;
    }
    ++bits; // for depth;
    bits += children.size();

    for (int i = 0; i < children.size(); ++i) {
        bits += children[i]->hash(depth + 1);
    }

    return bits;
}

bool Node::equals(Node &n, int depth) {
    if (depth > 0) {
        if (c != n.c) return false;
        if (t != n.t) return false;
        if (inSmaller != n.inSmaller) return false;
    }
    if (children.size() != n.children.size()) return false;

    for (int i = 0; i < children.size(); i++) {
        if (!children[i]->equals(*(n.children[i]), depth + 1)) {
            return false;
        }
    }
    return true;
}

int main() {
    root.t = false;
    root.c = '.';
    root.pointer = 0;

    ifstream smallerList("twl.txt");
    while(!smallerList.eof()) {
        string word;
        smallerList >> word;
        if (!smallerList.eof()) {
            smallerDict.insert(word);
        }
    }

    ifstream file("csw.txt");
    while(!file.eof()) {
        string word;
        file >> word;
        bool inSmaller = smallerDict.count(word);
        if (!file.eof()) root.pushword("", word, inSmaller);
    }

    nodeIndex = 0;
    root.print("");
    cout << "number of nodes: " << nodeIndex << endl;
    cout << "number of node structs: " << nodes.size() << endl;
    minimize();
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
