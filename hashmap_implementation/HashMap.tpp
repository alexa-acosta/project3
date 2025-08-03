#include <stdexcept>
#include <cmath> 
#include <iostream> 
#include <algorithm>
#include "HashMap.h" 

template <typename K, typename V>
size_t HashMap<K, V>::getNextPrime(size_t currentCapacity) const {
    size_t newCapacity = currentCapacity * 2 + 1;
    while (true) {
        bool isPrime = true;
        for (size_t i = 2; i * i <= newCapacity; ++i) {
            if (newCapacity % i == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            return newCapacity;
        }
        newCapacity += 2;
    }
}

template <typename K, typename V>
size_t HashMap<K, V>::hash(const K& key) const {
    return std::hash<K>{}(key) % table.size();
}

template <typename K, typename V>
HashMap<K, V>::HashMap(size_t initialCapacity) : numElements(0) {
    if (initialCapacity < 1) {
        throw std::invalid_argument("Capacity must be positive.");
    }
    size_t primeCapacity = getNextPrime(initialCapacity);
    table.resize(primeCapacity); 
}

template <typename K, typename V>
void HashMap<K, V>::resizeAndRehash() {
    size_t oldCapacity = table.size();
    size_t newCapacity = getNextPrime(oldCapacity);
    
    std::vector<std::list<std::pair<K, V>>> newTable(newCapacity);

    for (const auto& bucket : table) {
        for (const auto& pair : bucket) {
            size_t newIndex = std::hash<K>{}(pair.first) % newCapacity;
            newTable[newIndex].push_back(pair);
        }
    }

    table = std::move(newTable);
}

template <typename K, typename V>
void HashMap<K, V>::insert(const K& key, const V& value) {
    if (static_cast<double>(numElements) / table.size() > LOAD_FACTOR_THRESHOLD) {
        resizeAndRehash();
    }

    size_t index = hash(key);
    
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    
    table[index].emplace_back(key, value);
    numElements++;
}

template <typename K, typename V>
bool HashMap<K, V>::search(const K& key, V& valueOut) const {
    size_t index = hash(key); 
    
    for (const auto& pair : table[index]) {
        if (pair.first == key) {
            valueOut = pair.second; 
            return true;
        }
    }
    return false; 
}

template <typename K, typename V>
bool HashMap<K, V>::remove(const K& key) {
    size_t index = hash(key); 
    
    for (auto it = table[index].begin(); it != table[index].end(); ++it) {
        if (it->first == key) {
            table[index].erase(it); 
            numElements--;
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
size_t HashMap<K, V>::size() const {
    return numElements;
}

template <typename K, typename V>
void HashMap<K, V>::clear() {
    for (auto& bucket : table) {
        bucket.clear();
    }
    numElements = 0;
}

template <typename K, typename V>
void HashMap<K, V>::printStats() const {
    std::cout << "HashMap Statistics:" << std::endl;
    std::cout << "  Current Capacity: " << table.size() << std::endl;
    std::cout << "  Number of Elements: " << numElements << std::endl;
    std::cout << "  Load Factor: " << static_cast<double>(numElements) / table.size() << std::endl;

    size_t totalChainLength = 0;
    size_t longestChain = 0;
    size_t emptyBuckets = 0;

    for (const auto& bucket : table) {
        totalChainLength += bucket.size();
        if (bucket.size() > longestChain) {
            longestChain = bucket.size();
        }
        if (bucket.empty()) {
            emptyBuckets++;
        }
    }
    
    std::cout << "  Average Chain Length: " << static_cast<double>(totalChainLength) / table.size() << std::endl;
    std::cout << "  Longest Chain Length: " << longestChain << std::endl;
    std::cout << "  Number of Empty Buckets: " << emptyBuckets << std::endl;
}
