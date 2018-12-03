#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

vector<string> SplitIntoWords(const string& s)
{
    vector<string> vs;
    auto begit = s.begin();
    auto endit = s.begin();
    while (endit != s.end())
    {
        endit = find(begit, s.end(), ' ');
        vs.emplace_back(begit,endit);
        begit = endit;
        if (begit != s.end()) begit++;
    }
    return vs;
}
/*
int main() {
  string s = "C Cpp Java Python D G++";

  vector<string> words = SplitIntoWords(s);
  cout << words.size() << " ";
  for (auto it = begin(words); it != end(words); ++it) {
    if (it != begin(words)) {
      cout << "/";
    }
    cout << *it;
  }
  cout << endl;

  return 0;
}
*/