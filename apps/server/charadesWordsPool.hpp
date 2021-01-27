//
// Created by hubert on 27.01.2021.
//

#ifndef CHARADES_CHARADESWORDSPOOL_HPP
#define CHARADES_CHARADESWORDSPOOL_HPP

#include <memory>
#include <vector>
#include <random>

class charadesWordsPool {
    std::vector<std::string> pool;
    static std::unique_ptr<charadesWordsPool> instance;
    std::random_device rd;
    std::mt19937 mt;

public:
    void operator=(const charadesWordsPool &) = delete;
    charadesWordsPool(charadesWordsPool &other) = delete;
    static charadesWordsPool& getInstance();
    charadesWordsPool();
    std::string getRandomWord();
};


#endif //CHARADES_CHARADESWORDSPOOL_HPP
