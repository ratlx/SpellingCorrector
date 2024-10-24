//
// Created by 小火锅 on 24-10-23.
//

#include "SpellingCorrectorForZhCN.h"
#include "Pinyin.h"

#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace WzhePinYin;

// 创建一个宽字符转换器
#ifdef _WIN32
// Windows uses UTF-16
wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
#else
// Other systems use UTF-8
wstring_convert<codecvt_utf8<wchar_t>> converter;
#endif



void SpellingCorrectorForZhCN::addWord_ZhCN(const std::string &word, const std::string &pinyin){
    add_or_count_word(pinyin);
    if(!ZhCN_Dictionary.contains(word))
        ZhCN_Dictionary[word] = 1;
    pinyinToChinese[pinyin].push_back(word);
    chineseToPinyin[word].push_back(pinyin);
}

void SpellingCorrectorForZhCN::countWord(const std::string &word){
    for(auto& pinyin : chineseToPinyin[word])
        add_or_count_word(pinyin);
    ZhCN_Dictionary[word]++;
}

void SpellingCorrectorForZhCN::load(const std::string &filename) {


    string word, pinyin;
    wstring w_word;
    ifstream file(filename.c_str(), ios_base::in);

    while(file >> word >> pinyin) {
        bool daily_word = true;
        wordList w_py  {""};       //检测是否是常用字
        w_word = converter.from_bytes(word);
        for(auto wch : w_word) {
            if(!Pinyin::IsChinese(wch)) {
                daily_word = false;
                continue;
            }
            string word_t = converter.to_bytes(wch);
            if(!chineseToPinyin.contains(word_t)) {
                auto pinyin_t = Pinyin::GetPinyins(wch);
                for(auto& py : pinyin_t) {
                    addWord_ZhCN(word_t, py);
                }
            }
        }
        if(daily_word) {
            addWord_ZhCN(word, pinyin);
        }
    }
}

void SpellingCorrectorForZhCN::train(const std::string &filename){
    ifstream file(filename.c_str(), ios::in);
    string word, line, character;
    while(getline(file, line)) {
        auto w_line = converter.from_bytes(line);
        word.clear();
        for(auto wch : w_line) {
            if(!Pinyin::IsChinese(wch)) {
                word.clear();
                continue;
            }
            character = converter.to_bytes(wch);
            word += character;
            countWord(character);
            if(word.length() > 12)
                word.erase(word.begin(), word.begin() + 3);
            string word_t = word;
            while(word_t.size() > 3) {
                if(ZhCN_Dictionary.contains(word_t))
                    countWord(word_t);
                word_t.erase(word_t.begin(), word_t.begin() + 3);
            }
        }
    }
}

pair<string, string> SpellingCorrectorForZhCN::ZhCorrect(const std::string &word){
    auto sortByZhCN_Dictionary = [this](const std::string &w1, const std::string &w2) {
        return ZhCN_Dictionary[w1] < ZhCN_Dictionary[w2];
    };
    wordList div, res_1;
    int num = 1;
    for(; num <= word.length(); num++) {
        div.resize(num);
        res_1.resize(num);
        auto s_len = word.length() / num;
        auto remainder = word.length() % num;  // 处理余数部分
        for (string::size_type i = 0; i < num; ++i) {
            auto start = i * s_len + min(i, remainder);
            auto len = s_len + (i < remainder ? 1 : 0);

            div[i] = word.substr(start, len);  // 截取字符串
        }
        int cnt = 0;
        for(; cnt < num; cnt++) {
            res_1[cnt] = correct(div[cnt], false, true, true, true);
            if(res_1[cnt].empty())
                break;
        }
        if(cnt == num)
            break;
    }
    string ans_1;
    if(num != word.length() + 1)
        for(auto& r : res_1)
            ans_1 += *max_element(pinyinToChinese[r].begin(), pinyinToChinese[r].end(), sortByZhCN_Dictionary);


    string candidates, ans_2, word_t, res_2, tmp;
    int c_num_2 = 0;
    for(auto ch : word) {
        word_t += ch;
        auto res_t = correct(word_t, false, true, true, true);
        if(res_t.empty())
            res_t = correct(word_t, true, true, true, true);
        if(res_t == res_2)
            tmp += ch;
        else {
            if(!res_t.empty())
                tmp.clear();

            res_2 = res_t;
        }
        if(res_2.empty()) {
            ans_2 += candidates;
            c_num_2++;
            candidates.clear();
            word_t = tmp + ch;
            tmp.clear();
            continue;
        }
        candidates = *max_element(pinyinToChinese[res_2].begin(), pinyinToChinese[res_2].end(), sortByZhCN_Dictionary);
    }
    if(!candidates.empty())
        ans_2 += candidates;
    return {ans_1, ans_2};
}

