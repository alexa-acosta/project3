cmake_minimum_required(VERSION 3.10)
project(project3)

set(CMAKE_CXX_STANDARD 17)

include_directories(hashmap_implementation)

add_executable(project3 main.cpp
        trie_implementation/trie.cpp
        trie_implementation/trie.h)