#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <algorithm> 
#include <functional>
#include "hashmap_implementation/HashMap.h"
#include "trie_implementation/trie.h"

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
        std::string cell;
        std::string county, state, population;
        
        std::getline(ss, county, ',');
        county = trim(county);

        std::getline(ss, state, ',');
        state = trim(state);
        //skip to 2020 population data column
        for (int i = 0; i < 29; i++)
        {
            std::getline(ss, cell, ',');
        }
        std::getline(ss, population, ',');
        population = trim(population);

        if (!county.empty() && !state.empty()) {
            data.push_back({county, state, population});
        }
    }
    file.close();
    return data;
}


//
// Trie Functions
//

//Inserts CSV Data into a new Trie and returns the Trie
Trie useTrie(std::vector<CountyData>& dataset)
{
    Trie countyTrie;
    std::cout << "Inserting All County Data..." << std::endl;

    auto start_trie_time = std::chrono::high_resolution_clock::now();
    for (auto& row : dataset)
    {
        countyTrie.insert(row.countyName, row.stateName, row.population);
    }
    auto end_trie_time = std::chrono::high_resolution_clock::now();

    auto duration_trie_insert = std::chrono::duration_cast<std::chrono::milliseconds>(end_trie_time - start_trie_time);
    std::cout << "Total Insertion Time: " << duration_trie_insert.count() << " ms" << std::endl;
    std::cout << "Average Insertion Time: " << (double)duration_trie_insert.count() / dataset.size() << " ms" << std::endl;

    return countyTrie;
}

//Takes in a Trie and county name, performs exact search, and displays results and time
void trieExactSearch(Trie& countyTrie, std::string& county_name)
{
    std::unordered_map<std::string, std::string> state_populations;
    std::cout << "Exact Searching..." << std::endl;

    auto start_trie_time = std::chrono::high_resolution_clock::now();
    state_populations = countyTrie.searchFull(county_name);
    auto end_trie_time = std::chrono::high_resolution_clock::now();
    auto duration_trie_search_fullkey = std::chrono::duration_cast<std::chrono::milliseconds>(end_trie_time - start_trie_time);
    std::cout << "Total Exact Search Time " << duration_trie_search_fullkey.count() << " ms " << std::endl;

    if (state_populations.empty())
    {
        std::cout << "Could Not Find: " << county_name << std::endl;

    }
    else
    {
        std:: cout << "Found Entry(s): " << county_name << std::endl;
        //put logic here
        std::cout << "Print Results? (y/n)" << std::endl;
        char choice;
        std::cin >> choice;

        if (choice == 'y')
        {
            std::cout << "Matching Results: " << state_populations.size() << std::endl;
            std::cout << "County Populations as of 2020: " << std::endl;

            for (auto& pair : state_populations)
            {
                std::string state = pair.first;
                std::string population = pair.second;
                std::cout << county_name << ", " << state << ", Population: " << population << std::endl;
            }
        }
    }
    std::cout << "\nExact Search Complete" << endl;
}

//Takes in a Trie and county prefix, performs prefix search, and displays results and time
void triePrefixSearch(Trie& countyTrie, std::string& county_prefix)
{
    std::cout << "\nPrefix Searching..." << std::endl;
    std::vector<std::string> matching_counties;
    auto start_trie_time = std::chrono::high_resolution_clock::now();

    matching_counties = countyTrie.searchPrefix(county_prefix);

    auto end_trie_time = std::chrono::high_resolution_clock::now();
    auto duration_trie_search_prefixkey = std::chrono::duration_cast<std::chrono::milliseconds>(end_trie_time - start_trie_time);
    std::cout << "Total Prefix Search Time " << duration_trie_search_prefixkey.count() << " ms " << std::endl;

    if (matching_counties.empty())
    {
        std::cout << "Could Not Find Entry(s) with Prefix: " << county_prefix << std::endl;
    }
    else
    {
        std:: cout << "Found Entry(s) with Prefix: " << county_prefix << std::endl;
        //put logic here
        std::cout << "Print Results? (y/n)" << std::endl;
        char choice;
        std::cin >> choice;

        if (choice == 'y')
        {
            std::cout << "Matching Counties: " << matching_counties.size() << std::endl;
            for (auto& county : matching_counties)
            {
                std::cout << county << std::endl;
            }
        }
    }
    std::cout << "\nPrefix Search Complete" << endl;

}

void trieInsert(Trie& countyTrie)
{
    std::string county_insert_name;
    std::string county_insert_state;
    std::string county_insert_population;

    cout << "\nPlease input the requested county details" << std::endl;
    cout << "Enter County Name: " << std::endl;
    std::getline(std::cin >> std::ws, county_insert_name);
    cout << "Enter County State: " << std::endl;
    std::cin >> county_insert_state;
    cout << "Enter County Population: " << std::endl;
    std::cin >> county_insert_population;
    std::cout << "Inserting New County..." << std::endl;

    auto start_trie_time = std::chrono::high_resolution_clock::now();
    countyTrie.insert(county_insert_name, county_insert_state, county_insert_population);
    auto end_trie_time = std::chrono::high_resolution_clock::now();
    auto duration_trie_search_prefixkey = std::chrono::duration_cast<std::chrono::milliseconds>(end_trie_time - start_trie_time);
    std::cout << "Total Insertion Time " << duration_trie_search_prefixkey.count() << " ms " << std::endl;
    std::cout << "\nInsertion Complete" << endl;

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

    //
    // Trie Performance
    //

    std::cout << "\n--- Trie Performance ---" << std::endl;
    //Use Tree
    Trie countyTrie = useTrie(dataset);


    std::cout << "\nTesting Search (Prefix Search and Full countyName search)..." << std::endl;
    //Search for exact match
    //In menu user should be able to input county_name
    std::string county_name = "Monroe County";
    trieExactSearch(countyTrie, county_name);

    //prefix key search
    //In menu user should be able to input county_prefix
    std::string county_prefix = "Mon";
    triePrefixSearch(countyTrie, county_prefix);

    //insert
    //in menu user should be able to input
    trieInsert(countyTrie);

    //test
    std::string test_county_name = "Example Project County";
    trieExactSearch(countyTrie, test_county_name);

    return 0;
}
