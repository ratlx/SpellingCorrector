//
// cloned from https://github.com/felipefarinon/spellingcorrector
// modified by 小火锅 on 24-10-23.
//
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include "SpellingCorrector.h"

using namespace std;

bool sortBySecond(const pair<std::string, int>& left, const pair<std::string, int>& right) {
    return left.second < right.second;
}

char filterNonAlphabetic(char &letter) {
    if (letter < 0)
        return '-';
    if (isalpha(letter))
        return (char) tolower(letter);
    return '-';
}

void SpellingCorrector::load(const std::string &filename) {
    ifstream file(filename.c_str(), ios_base::binary | ios_base::in);

    file.seekg(0, ios_base::end);
    auto length = file.tellg();

    file.seekg(0, ios_base::beg);

    string data(static_cast<std::size_t>(length), '\0');

    file.read(&data[0], length);

    transform(data.begin(), data.end(), data.begin(), filterNonAlphabetic);

    for (string::size_type i = 0; i != string::npos;) {
        const auto firstNonFiltered = data.find_first_not_of('-', i + 1);
        if (firstNonFiltered == string::npos)
            break;

        const auto end = data.find('-', firstNonFiltered);
        dictionary[data.substr(firstNonFiltered, end - firstNonFiltered)]++;

        i = end;
    }
}

string SpellingCorrector::correct(const std::string &word,
    bool del, bool trpos, bool alt, bool ins) {
    wordList result;
    oppDictionary candidates;

    if (dictionary.count(word)) {
        return word;
    }

    edits(word, result, del, trpos, alt, ins);
    known(result, candidates);

    if (!candidates.empty()) {
        //return max_element(candidates.begin(), candidates.end(), sortBySecond)->first;
        return candidates.begin()->second;
    }

    for (auto& str : result) {
        wordList subResult;

        edits(str, subResult, del, trpos, alt, ins);
        known(subResult, candidates);
    }

    if (!candidates.empty()) {
        //return max_element(candidates.begin(), candidates.end(), sortBySecond)->first;
        return candidates.begin()->second;
    }

    return "";
}

void SpellingCorrector::known(wordList &results, oppDictionary &candidates) {
    auto end = dictionary.end();

    for (auto& word : results) {
        auto value = dictionary.find(word);

        if (value != end)
            //candidates[value->first] = value->second;
            candidates[value->second] = value->first;

    }
}

void SpellingCorrector::edits(const std::string &word, wordList &result,
    bool del, bool trpos, bool alt, bool ins) {
    if(word.empty()) return;
    string::size_type start = del ? 0 : 1;
    if(del)
        for (string::size_type i = start; i < word.size(); i++)
            result.emplace_back(word.substr(0, i) + word.substr(i + 1)); // deletions
    if(trpos)
        for (string::size_type i = start; i < word.size() - 1; i++)
            result.emplace_back(word.substr(0, i) + word[i + 1] + word[i] + word.substr(i + 2)); // transposition

    for (char j = 'a'; j <= 'z'; ++j) {
        if(alt)
            for (string::size_type i = start; i < word.size(); i++)
                result.emplace_back(word.substr(0, i) + j + word.substr(i + 1)); // alterations
        if(ins)
            for (string::size_type i = start; i < word.size() + 1; i++)
                result.emplace_back(word.substr(0, i) + j + word.substr(i)); // insertion
    }
}

int SpellingCorrector::getFrequence(const std::string &word) {
    if(dictionary.count(word))
        return dictionary[word];
    return 0;
}

void SpellingCorrector::add_or_count_word(const std::string &word) {
    dictionary[word]++;
}


