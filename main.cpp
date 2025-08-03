#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <algorithm> 
#include <functional>
#include "hashmap_implementation/HashMap.h"

struct CountyData {
    std::string countyName;
    std::string stateName;
    std::string population;
};

std::string trim(const std::string& str) {
    std::string s = str;

    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));

    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());

    if (s.length() >= 2 && s.front() == '"' && s.back() == '"') {
        s = s.substr(1, s.length() - 2);
    }

    return s;
}

std::vector<CountyData> loadData(const std::string& filename) {
    std::vector<CountyData> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string county, state, population;
        
        std::getline(ss, county, ',');
        county = trim(county);

        std::getline(ss, state, ',');
        state = trim(state);

        std::getline(ss, population, ',');
        population = trim(population);

        if (!county.empty() && !state.empty()) {
            data.push_back({county, state, population});
        }
    }
    file.close();
    return data;
}

int main() {
    std::cout << "Loading dataset..." << std::endl;
    std::vector<CountyData> dataset = loadData("county_demographics.csv");
    if (dataset.empty()) {
        std::cerr << "Failed to load data. Exiting." << std::endl;
        return 1;
    }
    std::cout << "Successfully loaded " << dataset.size() << " entries." << std::endl;

    std::cout << "\n--- HashMap Performance ---" << std::endl;
    HashMap<std::string, std::string> countyMap;

    std::cout << "Testing insertion..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& row : dataset) {
        countyMap.insert(row.countyName, row.stateName);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_insert = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Total insertion time: " << duration_insert.count() << " ms" << std::endl;
    std::cout << "Average insertion time: " << (double)duration_insert.count() / dataset.size() << " ms per entry" << std::endl;
    
    countyMap.printStats();

    std::cout << "\nTesting search..." << std::endl;
    std::string valueOut;
    std::string existingKey = "Alachua County"; 
    
    start = std::chrono::high_resolution_clock::now();
    bool found_existing = countyMap.search(existingKey, valueOut);
    end = std::chrono::high_resolution_clock::now();
    auto duration_search_existing = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (found_existing) {
        std::cout << "Found '" << existingKey << "'. Value: '" << valueOut << "'. Search took " << duration_search_existing.count() << " us." << std::endl;
    } else {
        std::cout << "Did not find '" << existingKey << "'. Search took " << duration_search_existing.count() << " us. This is incorrect." << std::endl;
    }
    
    std::string nonExistingKey = "NonExistent County";
    start = std::chrono::high_resolution_clock::now();
    bool found_non_existing = countyMap.search(nonExistingKey, valueOut);
    end = std::chrono::high_resolution_clock::now();
    auto duration_search_non_existing = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (found_non_existing) {
        std::cout << "Found '" << nonExistingKey << "'. Search took " << duration_search_non_existing.count() << " us. This is incorrect." << std::endl;
    } else {
        std::cout << "Did not find '" << nonExistingKey << "'. Search took " << duration_search_non_existing.count() << " us." << std::endl;
    }

    std::cout << "\nTesting removal..." << std::endl;
    std::string keyToRemove = "Abbeville County";
    std::string keyToNotRemove = "Imaginary County";

    start = std::chrono::high_resolution_clock::now();
    bool removed_existing = countyMap.remove(keyToRemove);
    end = std::chrono::high_resolution_clock::now();
    auto duration_remove_existing = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (removed_existing) {
        std::cout << "Successfully removed '" << keyToRemove << "'. Removal took " << duration_remove_existing.count() << " us." << std::endl;
        if (!countyMap.search(keyToRemove, valueOut)) {
            std::cout << "Verified: '" << keyToRemove << "' is no longer found." << std::endl;
        } else {
            std::cout << "Error: '" << keyToRemove << "' was found after removal attempt." << std::endl;
        }
    } else {
        std::cout << "Failed to remove '" << keyToRemove << "'. Removal took " << duration_remove_existing.count() << " us. This is incorrect." << std::endl;
    }

    start = std::chrono::high_resolution_clock::now();
    bool removed_non_existing = countyMap.remove(keyToNotRemove);
    end = std::chrono::high_resolution_clock::now();
    auto duration_remove_non_existing = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (removed_non_existing) {
        std::cout << "Error: Successfully removed '" << keyToNotRemove << "'. Removal took " << duration_remove_non_existing.count() << " us. This is incorrect." << std::endl;
    } else {
        std::cout << "Correctly did not remove '" << keyToNotRemove << "'. Removal took " << duration_remove_non_existing.count() << " us." << std::endl;
    }
    
    countyMap.printStats();

    return 0;
}
