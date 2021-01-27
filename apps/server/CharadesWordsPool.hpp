//
// Created by hubert on 27.01.2021.
//

#ifndef CHARADES_CHARADESWORDSPOOL_HPP
#define CHARADES_CHARADESWORDSPOOL_HPP

#include <memory>
#include <vector>
#include <random>

class CharadesWordsPool {
    std::vector<std::string> pool;
    static std::unique_ptr<CharadesWordsPool> instance;
    std::random_device rd;
    std::mt19937 mt;

public:
    void operator=(const CharadesWordsPool &) = delete;
    CharadesWordsPool(CharadesWordsPool &other) = delete;
    static CharadesWordsPool& getInstance();
    CharadesWordsPool();
    std::string getRandomWord();
};


#endif //CHARADES_CHARADESWORDSPOOL_HPP
