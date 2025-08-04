#ifndef TRIE_H
#define TRIE_H

#include<string>
#include<unordered_map>
#include<vector>
using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool end_word;
    vector<string> states;

    TrieNode() : end_word(false) {}

    ~TrieNode()
    {
        for (auto& [key, value] : children)
        {
            delete value;
        }
    }
};

class Trie
{
private:
    TrieNode* root;

public:
    Trie ()
    {
        root = new TrieNode();
    }
    ~Trie()
    {
        delete root;
    }

    void insert(string& word, string& state);
    bool search(string& word, vector<string>& states_outgoing);
    bool hasPrefix(string& prefix);
};


#endif //TRIE_H
