#include "trie.h"
#include <iostream>
using namespace std;

void Trie::insert(const string& word, const string& state, const string& population)
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
    current->state_populations[state] = population;
}

unordered_map<string, string> Trie::searchFull(string& word)
{
    unordered_map<string, string> result;
    TrieNode* current = root;
    for (char ch : word)
    {
        if (current->children.find(ch) == current->children.end())
        {
            //cout << "In Search Fail" << endl;
            return result;
        }
        current = current->children[ch];
    }

    if (current != nullptr && current->end_word)
    {
        result = current->state_populations;
        return result;
    }
    return result;
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

bool Trie::isEmpty()
{
    return root->children.empty();
}