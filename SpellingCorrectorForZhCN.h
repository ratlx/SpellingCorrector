//
// Created by 小火锅 on 24-10-23.
//

#ifndef SPELLINGCORRECTORFORZHCN_H
#define SPELLINGCORRECTORFORZHCN_H

#include <map>
#include <unordered_map>
#include <vector>

#include "SpellingCorrector.h"


class SpellingCorrectorForZhCN :protected SpellingCorrector{
public:
    //int getFrequence(const std::string& word);
    void load(const std::string& word);
    std::pair<std::string, std::string> ZhCorrect(const std::string& word);
    void train(const std::string& filename);
    void addWord_ZhCN(const std::string& word, const std::string& pinyin);
    void countWord(const std::string& word);

    using Transform = std::unordered_map<std::string, wordList>;

    Transform transform;        //拼音--->>中文
    Transform opptransform;     //中文--->>拼音
    Dictionary ZhCN_Dictionary;

};



#endif //SPELLINGCORRECTORFORZHCN_H
