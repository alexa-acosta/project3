#ifndef TRIE_H
#define TRIE_H

#include<string>
#include<unordered_map>
#include<vector>
using namespace std;

class TrieNode
{
public:
    unordered_map<char, TrieNode*> children;
    bool end_word;
    unordered_map<string, string> state_populations;

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

    void insert(const string& word, const string& state, const string& population);
    unordered_map<string, string> searchFull(string& word);
    vector<string> searchPrefix(string& prefix);

    //helper functions
    void findEntries(TrieNode* current, string& prefix, vector<string>& results);
    bool isEmpty();
};


#endif //TRIE_H
