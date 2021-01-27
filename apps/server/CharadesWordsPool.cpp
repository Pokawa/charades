//
// Created by hubert on 27.01.2021.
//

#include "CharadesWordsPool.hpp"
#include <fstream>

std::unique_ptr<CharadesWordsPool> CharadesWordsPool::instance;
CharadesWordsPool &CharadesWordsPool::getInstance() {
    if (instance == nullptr) {
        instance = std::make_unique<CharadesWordsPool>();
    }
    return *instance;
}

CharadesWordsPool::CharadesWordsPool() : mt(rd()) {
    std::ifstream file{"words.txt"};

    while(!file.eof()) {
        std::string buff;
        std::getline(file, buff);
        pool.push_back(std::move(buff));
    }

    file.close();
}

std::string CharadesWordsPool::getRandomWord() {
    auto maxIndex = static_cast<int>(pool.size()) - 1;
    std::uniform_int_distribution<int> dist{0, maxIndex};
    return pool.at(dist(mt));
}


