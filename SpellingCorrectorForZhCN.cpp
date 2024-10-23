//
// Created by 小火锅 on 24-10-23.
//

#include "SpellingCorrectorForZhCN.h"
#include <fstream>
#include "Pinyin.h"
#include <string>
#include <locale>
#include <codecvt>
#include <iostream>
#include <algorithm>
using namespace std;
using namespace WzhePinYin;

// 创建一个宽字符转换器
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

/*vector<string> division(string word, int num) {

}*/

bool SpellingCorrectorForZhCN::sortByZhCN_Dictionary(const std::string &w1, const std::string &w2){
    return ZhCN_Dictionary[w1] < ZhCN_Dictionary[w2];
}


void SpellingCorrectorForZhCN::addWord_ZhCN(const std::string &word, const std::string &pinyin){
    addWord(pinyin);
    ZhCN_Dictionary[word]++;
    transform[pinyin].push_back(word);
    opptransform[word].push_back(pinyin);
}

void SpellingCorrectorForZhCN::countWord(const std::string &word){
    for(auto& pinyin : opptransform[word])
        addWord(pinyin);
    ZhCN_Dictionary[word]++;
}

void SpellingCorrectorForZhCN::load(const std::string &filename1, const std::string &filename2) {
    ifstream file(filename1.c_str(), ios_base::in);

    string word, pinyin;
    file >> word;
    auto w_word = converter.from_bytes(word);
    for(auto character : w_word) {
        if(!Pinyin::IsChinese(character))
            continue;
        auto pinyin_t = Pinyin::GetPinyins(character);
        bool first = true;
        string word_t = converter.to_bytes(character);
        for(auto& py : pinyin_t) {
            if(first) {
                addWord_ZhCN(word_t, py);
                first = false;
            }
            else
                opptransform[word_t].push_back(py);
        }
    }

    file.close();
    file.open(filename2.c_str(), ios_base::in);

    while(file >> word >> pinyin) {
        bool daily_word = true;
        Vector w_py  {""};       //检测是否是常用字
        w_word = converter.from_bytes(word);
        for(auto wch : w_word) {
            if(!Pinyin::IsChinese(wch)) {
                daily_word = false;
                break;
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
                if(ZhCN_Dictionary.count(word_t))
                    countWord(word_t);
                word_t.erase(word_t.begin(), word_t.begin() + 3);
            }
        }
    }
}

std::string SpellingCorrectorForZhCN::ZhCorrect(const std::string &word){
    Vector div, res;
    int num = 1;
    for(; num <= word.length(); num++) {
        div.resize(num);
        res.resize(num);
        int s_len = word.length() / num;
        int remainder = word.length() % num;  // 处理余数部分
        for (int i = 0; i < num; ++i) {
            int start = i * s_len + min(i, remainder);
            int len = s_len + (i < remainder ? 1 : 0);

            div[i] = word.substr(start, len);  // 截取字符串
        }
        int cnt = 0;
        for(; cnt < num; cnt++) {
            res[cnt] = correct(div[cnt]);
            if(res[cnt].empty())
                break;
        }
        if(cnt == num)
            break;
    }
    if(num == word.length() + 1)
        return "";
    string ans;
    for(auto& r : res) {
        ans += *max_element(transform[r].begin(), transform[r].end(),
                    [this](const std::string &w1, const std::string &w2) {
                        return this->sortByZhCN_Dictionary(w1, w2);
                    });
    }
    return ans;
}

