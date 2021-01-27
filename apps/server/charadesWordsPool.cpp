//
// Created by hubert on 27.01.2021.
//

#include "charadesWordsPool.hpp"
#include <fstream>

std::unique_ptr<charadesWordsPool> charadesWordsPool::instance;
charadesWordsPool &charadesWordsPool::getInstance() {
    if (instance == nullptr) {
        instance = std::make_unique<charadesWordsPool>();
    }
    return *instance;
}

charadesWordsPool::charadesWordsPool() : mt(rd()) {
    std::ifstream file{"words.txt"};

    while(!file.eof()) {
        std::string buff;
        std::getline(file, buff);
        pool.push_back(std::move(buff));
    }

    file.close();
}

std::string charadesWordsPool::getRandomWord() {
    auto maxIndex = static_cast<int>(pool.size()) - 1;
    std::uniform_int_distribution<int> dist{0, maxIndex};
    return pool.at(dist(mt));
}


