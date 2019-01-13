#include <algorithm>
#include <string>
#include <vector>

#include "profile.h"
#include "test_runner.h"

using namespace std;

class Learner {
 private:
//  vector<string> dict;
  set<string> dictionary;

 public:
  int Learn(const vector<string>& words) {
      int newWords = dictionary.size();
      dictionary.insert(words.begin(), words.end());
      newWords = dictionary.size() - newWords;
      return newWords;
/*    int newWords = 0;
    for (const auto& word : words) {
      if (find(dict.begin(), dict.end(), word) == dict.end()) {
        ++newWords;
        dict.push_back(word);
      }
    }
    return newWords;*/
  }

  vector<string> KnownWords() {
    return {dictionary.begin(), dictionary.end()};
/*    sort(dict.begin(), dict.end());
    dict.erase(unique(dict.begin(), dict.end()), dict.end());
    return dict;*/
  }
};


int main() {
  Learner learner;
  string line;
 // while (getline(cin, line)) {
  vector<string> Lines;
  for (int i = 0; i < 3000; ++i)
  {
      Lines.push_back("Word" + to_string(i));
      Lines.push_back("Word" + to_string(i));
  }
  {
    LOG_DURATION("Learn");
  for (const auto& line : Lines)
  {
    vector<string> words;
    stringstream ss(line);
    string word;
    while (ss >> word) {
      words.push_back(word);
    }
    /*cout << */learner.Learn(words)/* << "\n"*/;
  }
  }
  cout << "=== known words ===\n";
  {
      LOG_DURATION("Known");
  for (auto word : learner.KnownWords()) {
   /* cout <<*/ word /*<< "\n"*/;
  }
  }
}
