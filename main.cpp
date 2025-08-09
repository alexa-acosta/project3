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

void displayMenu()
{
    std::cout << "\n=============== Menu ===============" << std::endl;
    std::cout << "1. Load dataset from file" << std::endl;
    std::cout << "2. Load into Trie" << std::endl;
    std::cout << "3. Load into Hashmap " << std::endl;
    std::cout << "4. Insert new Trie entry" << std::endl;
    std::cout << "5. Insert new Hashmap entry" << std::endl;
    std::cout << "6. Search for exact match in Trie" << std::endl;
    std::cout << "7. Search for prefix match in Trie" << std::endl;
    std::cout << "8. Search for exact match in Hashmap" << std::endl;
    std::cout << "9. Exit" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "Please enter a number from 1-9 as your choice: " << std::endl;

}

// Trie Functions

//Inserts CSV Data into a new Trie and returns the Trie
void useTrie(Trie& countyTrie, std::vector<CountyData>& dataset)
{
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

}

//Takes in a Trie and county name, performs exact search, and displays results and time
void trieExactSearch(Trie& countyTrie, std::string& county_name)
{
    std::unordered_map<std::string, std::string> state_populations;
    std::cout << "Exact Searching for... '" << county_name << "'" << std::endl;

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
    std::cout << "Exact Search Complete" << endl;
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
    std::cout << "Prefix Search Complete" << endl;

}

//Takes in a Trie, and requests user for county data, and inserts a new county and displays time
void trieInsert(Trie& countyTrie)
{
    std::string county_insert_name;
    std::string county_insert_state;
    std::string county_insert_population;

    cout << "\nPlease input the requested county details" << std::endl;
    cout << "Enter County Name: " << std::endl;
    std::getline(std::cin >> std::ws, county_insert_name);
    cout << "Enter County State (Format: XX): " << std::endl;
    std::cin >> county_insert_state;
    cout << "Enter County Population: " << std::endl;
    std::cin >> county_insert_population;
    std::cout << "Inserting New County..." << std::endl;

    auto start_trie_time = std::chrono::high_resolution_clock::now();
    countyTrie.insert(county_insert_name, county_insert_state, county_insert_population);
    auto end_trie_time = std::chrono::high_resolution_clock::now();
    auto duration_trie_search_prefixkey = std::chrono::duration_cast<std::chrono::milliseconds>(end_trie_time - start_trie_time);
    std::cout << "Total Insertion Time " << duration_trie_search_prefixkey.count() << " ms " << std::endl;
    std::cout << "Insertion Complete" << endl;

}

// Hashmap Functions

void useHashMap(HashMap<std::string, std::string>& countyMap, std::vector<CountyData>& dataset)
{

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
}

void hashmapExactSearch(HashMap<std::string, std::string>& countyMap, std::string& county_name)
{
    std::cout << "\nTesting search..." << std::endl;
    std::string valueOut;
    std::string existingKey = "Alachua County";

    auto start = std::chrono::high_resolution_clock::now();
    bool found_existing = countyMap.search(county_name, valueOut);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_search_existing = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (found_existing) {
        std::cout << "Found '" << county_name << "'. State: '" << valueOut << "'. Search took " << duration_search_existing.count() << " us." << std::endl;
    } else {
        std::cout << "Did not find '" << county_name << "'. Search took " << duration_search_existing.count() << " us." << std::endl;
    }

}

void hashmapInsert(HashMap<std::string, std::string>& countyMap)
{
    std::string county_insert_name;
    std::string county_insert_state;

    cout << "\nPlease input the requested county details" << std::endl;
    cout << "Enter County Name: " << std::endl;
    std::getline(std::cin >> std::ws, county_insert_name);
    cout << "Enter County State (Format: XX): " << std::endl;
    std::cin >> county_insert_state;
    std::cout << "Inserting New County..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    countyMap.insert(county_insert_name, county_insert_state);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_insert = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Total insertion time: " << duration_insert.count() << " ms" << std::endl;
}

int main() {

    std::vector<CountyData> dataset;
    Trie countyTrie;
    HashMap<std::string, std::string> countyMap;

    int choice = 0;

    while (choice != 9)
    {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
            {
                dataset = loadData("county_demographics.csv");
                std::cout << "Loading dataset..." << std::endl;
                if (!dataset.empty())
                {
                    std::cout << "Successfully loaded " << dataset.size() << " entries." << std::endl;
                }
                break;
            }
            case 2:
            {
                if (dataset.empty())
                {
                    std::cout << "Dataset is empty. Please load a dataset first." << std::endl;
                }
                else
                {
                    useTrie(countyTrie, dataset);
                }
                break;
            }
            case 3:
            {
                if (dataset.empty())
                {
                    std::cout << "Dataset is empty. Please load a dataset first." << std::endl;
                }
                else
                {
                    useHashMap(countyMap, dataset);

                }
                break;
            }
            case 4:
            {
                if (dataset.empty())
                {
                    std::cout << "Dataset is empty. Please load a dataset first." << std::endl;
                }
                else
                {
                    trieInsert(countyTrie);
                }
                break;
            }
            case 5:
            {
                if (dataset.empty())
                {
                    std::cout << "Dataset is empty. Please load a dataset first." << std::endl;
                }
                else
                {
                    hashmapInsert(countyMap);
                }
                break;
            }
            case 6:
            {
                if (dataset.empty())
                {
                    std::cout << "Dataset is empty. Please load a dataset first." << std::endl;
                }
                else
                {
                    std::string county_name;
                    std::cout << "Enter County Name (Exact Search): " << std::endl;
                    std::getline(std::cin, county_name);
                    trieExactSearch(countyTrie, county_name);
                }
                break;
            }
            case 7:
            {
                if (dataset.empty())
                {
                    std::cout << "Dataset is empty. Please load a dataset first." << std::endl;
                }
                else
                {
                    std::string county_name;
                    std::cout << "Enter County Prefix (Prefix Search): " << std::endl;
                    std::getline(std::cin, county_name);
                    cout << county_name << std::endl;
                    triePrefixSearch(countyTrie, county_name);
                }
                break;
            }
            case 8:
            {
                if (dataset.empty())
                {
                    std::cout << "Dataset is empty. Please load a dataset first." << std::endl;
                }
                else
                {
                    std::string county_name;
                    std::cout << "Enter County Name (Exact Search): " << std::endl;
                    std::getline(std::cin, county_name);
                    cout << county_name << std::endl;
                    hashmapExactSearch(countyMap, county_name);
                }
                break;
            }
            case 9:
            {
                std::cout << "Exiting..." << std::endl;
                break;
            }
            case 10:
            {
                if (countyTrie.isEmpty()) {
                    std::cout << "The countyTrie is currently empty." << std::endl;
                } else {
                    std::cout << "The countyTrie contains data." << std::endl;
                }
                break;
            }
            default:
            {
                std::cout << "Invalid menu choice, please enter a valid number (1-9) from the menu." << std::endl;
                break;
            }

        }
    }
    return 0;
}
