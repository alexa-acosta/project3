#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <vector>
#include <list>
#include <utility>
#include <functional>
#include <stdexcept>

template <typename K, typename V>
class HashMap {
private:
    static constexpr double LOAD_FACTOR_THRESHOLD = 0.75;
    std::vector<std::list<std::pair<K, V>>> table;
    size_t numElements;

    size_t hash(const K& key) const;
    void resizeAndRehash();
    size_t getNextPrime(size_t currentCapacity) const;

public:
    HashMap(size_t initialCapacity = 101);
    
    void insert(const K& key, const V& value);
    bool search(const K& key, V& valueOut) const;
    
    size_t size() const;
    void clear();
    void printStats() const;
};

#include "HashMap.tpp"

#endif