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
    using Vector = std::vector<std::string>;
    using Dictionary =  std::unordered_map<std::string, int>;
    using oppDictionary = std::map<int, std::string, std::greater<>> ;

    int getFrequence(const std::string& word);
    void addWord(const std::string& word);
    void load(const std::string& filename);
    std::string correct(const std::string& word);
private:

    Dictionary dictionary;

    void edits(const std::string& word, Vector& result);
    void known(Vector& results, oppDictionary& candidates);


};

#endif
