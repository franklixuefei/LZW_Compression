#include <iostream>
#include <sstream>
#include "bitbuffer.h"
#include <fstream>
using namespace std;
struct TrieNode {
    enum { NumChars = 257 };
    bool isWord;
    unsigned short code;
    //    static int counter;
    TrieNode *letters[NumChars];
    TrieNode() {
        //counter++;
        isWord = false;
        isWord = 0;
        for ( int i = 0; i < NumChars; i += 1 ) {
            letters[i] = NULL;
        } // for
    }
    
}; // TrieNode

//int TrieNode::counter;

void insert( TrieNode &node, const std::string &word, unsigned short code, TrieNode**pos );


void DeleteTree( TrieNode *&node );

TrieNode* lookup(TrieNode* t, const std::string& w, int pos);


const int NUM_CHAR = 257;

/************ insert *************
 Purpose: 
 Adds word to the dictionary.
 
 Parameters:
 node: a trie structure passed as reference, in which
 word is to be added if it does not exist in the dictionary.
 
 word: the constant string which is to be added to node
 if not already existing in node.
 
 Returns: void.
 ************************************/
void insert( TrieNode &node, const string &word, unsigned short code, TrieNode* *pos ) {
    istringstream iss(word);
    unsigned char addChar;
    TrieNode **letter = node.letters, *aNode = &node;
    while (iss >> noskipws >> addChar) {
        int charLoc = addChar; // charLoc from 0 to 25.
        if (letter[charLoc]) {
            aNode = letter[charLoc];
            letter = letter[charLoc]->letters;
        } else {
            letter[charLoc] = new TrieNode;
            aNode = letter[charLoc];
            letter = letter[charLoc]->letters;
        } // if
    } // while
    aNode->code = code;
    aNode->isWord = true;
    pos[code] = aNode;
} // insert

TrieNode* lookup(TrieNode* t, const string& w, int pos) {
    //cout << pos << endl;
    if (t == NULL) return NULL;
    if (w.size() == pos) {
        if (t->isWord) return t;
        else return NULL;
    } else {
        int new_pos = pos + 1;
        return lookup(t->letters[w[pos]], w, new_pos);
    }
}

void DeleteTree( TrieNode *&node ) {
    for (int i = 0; i < NUM_CHAR; ++i) {
        if (node) {
            TrieNode *letter = node->letters[i];
            if (letter) DeleteTree(letter);
        } // if
    } // for
    delete node;
} // DeleteTree




int main(int argc, char* argv[]) {
    
    if (argc == 3) {
        TrieNode** pos = new TrieNode*[1024];
        TrieNode *trie = new TrieNode();
        unsigned short counter = 0;
        bool beginOverwriting = false;
        //TrieNode::counter--;
        //insert(*trie, "\n", 12);
        //        insert(*trie, "ab b", 2);
        //        cout << trie->letters[10]->isWord << endl;
        //        cout << trie->letters[97]->letters[98]->letters[32]->letters[98]->isWord << endl;
        //        cout << lookup(trie, "\n", 0) << endl;
        //        cout << lookup(trie, "ab b", 0) << endl;
        BitBuffer bit;
        //        char c = 'c';
        //        for (int i = 0; i < 8; ++i) {
        //            bit->addBit(c>>(7-i));
        //        }
        //        //    bit->addBit('d');
        //        cout << bit->byteReady() << endl;
        //        cout << bit->getByte() << endl;
        //        cout << bit->isEmpty() << endl;
        for (short i = 0; i < 256; ++i) {
            string s = "";
            s += i;
            //cout << "S: " << "|"<< s << "|" << endl;
            insert(*trie, s, counter, pos);
            counter++;
        }
        counter++; // means reserve letter[256] for eof char.
        
        //    for (int i = 0; i < 256; ++i) {
        //        cout << trie->letters[i]->code << endl;
        //    }
        //TrieNode::counter++;
        //cout << TrieNode::counter << endl;
        //    cout << lookup(trie, " ", 0)->code << endl;
        //        ifstream iff(argv[1]);
        //        unsigned char c;
        //        while(true) {
        //            
        //        }
        //   }
        ifstream iff(argv[1]);
        unsigned char c;
        c = iff.get();
        string s = "";
        string temp = "";
        s += c;
        temp += c;
        
        while (iff.good()) {
            
            c = iff.peek();
            TrieNode *prevT = lookup(trie, temp, 0);
            if (iff.good()) {
                //cout << "temp: "+temp << endl;
                //TrieNode *prevT = lookup(trie, temp, 0);
                temp += c;
                //cout << "temp2: "+temp << endl;
                TrieNode *t = lookup(trie, temp, 0);
                if (t) {
                    c = iff.get();
                    s += c;
                    
                } else { // now s contains longest prefix possible
                    // output code
                    //cout << "|"+temp+"|" << endl;
                    //cout << prevT->code << endl;
                    for (int i = 0; i < 10; ++i) {
                        
                        bit.addBit(prevT->code>>(9-i));
                    }
                    unsigned char nc = iff.peek();
                    s += nc;
                    if (counter >= 1024) {counter = 257; beginOverwriting = true;}
                    //cout << counter << endl;
                    if (beginOverwriting) {
                        TrieNode* tempNode = pos[counter];
                        tempNode->isWord = false;
                        tempNode->code = 0;
                    }
                    insert(*trie, s, counter, pos);
                    counter++;
                    c = iff.get();
                    s = temp  = "";
                    s += c;
                    temp += c;
                }
            } else {
                for (int i = 0; i < 10; ++i) {
                    
                    bit.addBit(prevT->code>>(9-i));
                }
                
            }
        }
        for (int i = 0; i < 10; ++i) {
            bit.addBit(256>>(9-i));
        }
        
        ofstream off(argv[2]);
        while(!bit.isEmpty()) {
            off.put(bit.getByte());
        }
        DeleteTree(trie);
        delete [] pos;
        //    string s = "a";
        //    s += '\n';
        //    cout << s;
        //    cout << "abc" << endl;
    }
    return 0;
}

