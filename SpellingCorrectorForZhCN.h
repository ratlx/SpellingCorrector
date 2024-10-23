//
// Created by 小火锅 on 24-10-23.
//

#ifndef SPELLINGCORRECTORFORZHCN_H
#define SPELLINGCORRECTORFORZHCN_H

#include <map>
#include <unordered_map>
#include <vector>

#include "SpellingCorrector.h"


class SpellingCorrectorForZhCN :private SpellingCorrector{
public:
    //int getFrequence(const std::string& word);
    void load(const std::string& character, const std::string& word);
    std::string ZhCorrect(const std::string& word);
    void train(const std::string& filename);
    void addWord_ZhCN(const std::string& word, const std::string& pinyin);
    void countWord(const std::string& word);
private:
    using Transform = std::unordered_map<std::string, Vector>;

    Transform transform;
    Transform opptransform;
    Dictionary ZhCN_Dictionary;

    bool sortByZhCN_Dictionary(const std::string& w1, const std::string& w2);

};



#endif //SPELLINGCORRECTORFORZHCN_H
