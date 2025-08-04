#include "trie.h"


void Trie::insert(string& word)
{
    TrieNode* current = root;
    for (char ch : word)
    {
        if (current->children.find(ch) == current->children.end())
        {
            current->children[ch] = new TrieNode();
        }
    }
    current->end_word = true;
}

bool Trie::search (string& word)
{
    TrieNode* current = root;
    for (char ch : word)
    {
        if (current->children.find(ch) == current->children.end())
        {
            return false;
        }
        current = current->children[ch];
    }
    return (current != nullptr && current->end_word);
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