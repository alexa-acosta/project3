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


bool Trie::searchFull(string& word, vector<string>& states_outgoing)
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


vector<string> Trie::searchPrefix(string& prefix)
{
    vector<string> results;
    TrieNode* current = root;
    for (char ch : prefix)
    {
        if (current->children.find(ch) == current->children.end())
        {
            return results;
        }
        current = current->children[ch];
    }

    findEntries(current, prefix, results);
    return results;
}

void Trie::findEntries(TrieNode* current, string& prefix, vector<string>& results)
{
    if (current->end_word)
    {
        results.push_back(prefix);
    }
    for (auto& [key, value] : current->children)
    {
        prefix.push_back(key);
        findEntries(value, prefix, results);
        prefix.pop_back();
    }
}