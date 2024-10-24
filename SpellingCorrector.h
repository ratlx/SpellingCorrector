//
// cloned from https://github.com/felipefarinon/spellingcorrector
// modified by 小火锅 on 24-10-23.
//

#ifndef SPELLINGCORRECTOR_H_
#define SPELLINGCORRECTOR_H_

#include <vector>
#include <map>
#include <unordered_map>

class SpellingCorrector
{
public:
    using wordList = std::vector<std::string>;
    using Dictionary =  std::unordered_map<std::string, int>;
    using oppDictionary = std::map<int, std::string, std::greater<>> ;

    int getFrequence(const std::string& word);
    void load(const std::string& filename);
    std::string correct(const std::string& word,bool del = true,
        bool trpos = true, bool alt = true, bool ins = true);

protected:
    void add_or_count_word(const std::string& word);

private:

    Dictionary dictionary;

    void edits(const std::string& word, wordList& result,bool del = true,
        bool trpos = true, bool alt = true, bool ins = true);
    void known(wordList& results, oppDictionary& candidates);
};

#endif
