//
// Created by 小火锅 on 24-10-23.
//
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include "SpellingCorrector.h"
#include "SpellingCorrectorForZhCN.h"


#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

double calculatePercentage(int part, int total) {
    if (total == 0) {
        return 0.0; // 避免除以零
    }
    return (static_cast<double>(part) / total) * 100;
}

void Correct(SpellingCorrector& corrector,const std::string& wrong, const std::string& expected)
{
    const bool isCorrect = corrector.correct(wrong) == expected;
    std::cout << "(" << wrong << ") == (" << expected << ") = (" << std::boolalpha << isCorrect << ")" << std::endl;
}

pair<string, bool> isCorrect(SpellingCorrector& corrector,const std::string& wrong, const std::string& expected) {
    string res = corrector.correct(wrong);
    return {res, res == expected};
}

void singleRequest(SpellingCorrector& corrector) {
    string request;
    while (request != "quit")
    {
        cout << "Enter a word\n";
        cin >> request;

        string correct(corrector.correct(request));

        if (!correct.empty())
            cout << "You meant: " << correct << "?\n\n\n";
        else
            cout << "No correction suggestion\n\n\n";
    }
}

void singleRequest_ZH(SpellingCorrectorForZhCN& corrector) {
    string request;
    while (request != "tuichu")
    {
        cout << "输入拼音\n";
        cin >> request;

        auto ans(corrector.ZhCorrect(request));

        if (!ans.first.empty() && !ans.second.empty())
            if(ans.first != ans.second)
                cout << "你的意思是 “" << ans.first << "” 还是 “" << ans.second << "” ?\n";
            else
                cout << "你的意思是 “" << (ans.first.empty() ? ans.second : ans.first) << "” ?\n";
        else
            cout << "555啥也找不到呢 :(\n";
    }
}
void spellTest(SpellingCorrector& corrector, const string& filename, bool verbose = false) {
    ifstream ipf(filename.c_str(), ios::in);

    if(!ipf) {
        cout << "fail at opening file" << endl;
        return ;
    }

    auto start = chrono::high_resolution_clock::now();
    int sum = 0, right_s = 0, unknown = 0;
    string line;

    while(getline(ipf, line)) {
        istringstream iss(line);
        string ans, request;
        iss >> ans;
        ans.erase(ans.end() - 1);
        if(corrector.getFrequence(ans) == 0)
            unknown++;

        while(iss >> request) {
            sum++;
            auto res = isCorrect(corrector, request, ans);
            if(res.second)
                right_s++;
            else {
                if(verbose) {
                    cout << "correction(" << request << ") =>"
                    << res.first << '(' << corrector.getFrequence(res.first)
                    << "); expected " << ans << '(' << corrector.getFrequence(ans) << ")\n";
                }
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;

    cout << calculatePercentage(right_s, sum) << "% of " << sum << " correct ("
    << calculatePercentage(unknown, sum) << "% unknown) at "
    << (double) sum / diff.count() << " words per second\n";
}


int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    //SpellingCorrector corrector;

    SpellingCorrectorForZhCN ZHcorrector;
    ZHcorrector.load("test_file/word.txt");
    ZHcorrector.train("test_file/train.zh");
    //cout << ZHcorrector.ZhCorrect("ysqidong");
    singleRequest_ZH(ZHcorrector);
    //corrector.load("big.txt");

    /*ofstream opf("out.txt", ios::out);
    for(auto& i : ZHcorrector.ZhCN_Dictionary) {
        if(i.second > 1 && ZHcorrector.chineseToPinyin[i.first].size() > 1) {
            opf << i.first << ' '<< i.second << endl;
            for(auto& j : ZHcorrector.chineseToPinyin[i.first])
                opf << j << endl;
        }
    }*/

    /*Correct("speling", "spelling"); // insert
    Correct("korrectud", "corrected"); // replace 2
    Correct("bycycle", "bicycle"); // replace
    Correct("inconvient", "inconvenient"); // insert 2
    Correct("arrainged", "arranged"); // delete
    Correct("peotry", "poetry"); // transpose
    Correct("peotryy", "poetry"); // transpose + delete
    Correct("word", "word"); // known
    Correct("quintessential", ""); // unknown*/

    //singleRequest(corrector);
    //spellTest(corrector,"test1.txt", false);
    //spellTest(corrector, "test2.txt", false);

    return 0;
}
