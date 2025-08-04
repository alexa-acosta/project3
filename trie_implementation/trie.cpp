#include "trie.h"
#include <iostream>
using namespace std;

void Trie::insert(string& word, string& state)
{
    TrieNode* current = root;
    for (char ch : word)
    {
        if (current->children.find(ch) == current->children.end())
        {
            current->children[ch] = new TrieNode();
        }
        current = current->children[ch];
    }
    current->end_word = true;

    current->states.push_back(state);
}


bool Trie::search (string& word, vector<string>& states_outgoing)
{
    TrieNode* current = root;
    for (char ch : word)
    {
        if (current->children.find(ch) == current->children.end())
        {
            cout << "In Search Fail" << endl;
            return false;
        }
        current = current->children[ch];
    }

    if (current != nullptr && current->end_word)
    {
        states_outgoing = current->states;
        return true;
    }
    return false;
}


bool Trie::hasPrefix(string& prefix)
{
    TrieNode* current = root;
    for (char ch : prefix)
    {
        if (current->children.find(ch) == current->children.end())
        {
            return false;
        }
        current = current->children[ch];
    }
    return true;
}