#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
      for (const auto& [key, value] : other.word_frequences)
      {
           word_frequences[key] += value;
      }
  }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    stringstream ss(line);
    string word;
    Stats result;
    while (ss >> word) {
        if (key_words.find(word) != key_words.end()) ++result.word_frequences[move(word)];
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(
  const set<string>& key_words, istream&& input
) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

#define NUM_THREADS 4

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    streamoff input_size = input.seekg(0, input.end).tellg();
    input.seekg(0);
    if (input_size <= 0) return Stats{};
    Stats result;
    stringstream ss;
    streamoff char_array_size = input_size / NUM_THREADS + 1 + 1; // плюс 1 из-за деления и плюс 1 из-за get
    char * chars = new char[char_array_size];
    vector<future<Stats>> futures;
    while(!input.eof()) {
        ss.clear();
        ss.str("");
        input.get(chars, char_array_size, '\0');
        ss << chars;
        if (input.gcount() > 0 && input.peek() != ' ' && input.peek() != '\n' && input.good()) {
            string tmp_str;
            input >> tmp_str;
            ss << tmp_str;
        }
        futures.push_back( async(ExploreKeyWordsSingleThread, ref(key_words), move(ss)));
    }
    delete [] chars;

    for (auto& f : futures)
    {
        result += f.get();
    }
    return result;
}


Stats ExploreKeyWordsOneThread(const set<string>& key_words, istream& input) {
    streamoff input_size = input.seekg(0, input.end).tellg();
    input.seekg(0);
    if (input_size <= 0) return Stats{};
    Stats result;
    stringstream ss;
    streamoff char_array_size = input_size / NUM_THREADS + 1 + 1; // плюс 1 из-за деления и плюс 1 из-за get
    char * chars = new char[char_array_size];
    while(!input.eof()) {
        ss.clear();
        ss.str("");
        input.get(chars, char_array_size, '\0');
        ss << chars;
        if (input.gcount() > 0 && input.peek() != ' ' && input.peek() != '\n' && input.good()) {
            string tmp_str;
            input >> tmp_str;
            ss << tmp_str;
        }
        result += ExploreKeyWordsSingleThread(key_words, move(ss));
    }
    delete [] chars;
    return result;
}

void TestBasic() {
    {
      const set<string> key_words = {"yangle", "rocks", "sucks", "all"};
      stringstream ss;
      for (int i = 0; i < 100000; ++i) {
      ss << "this new yangle service really rocks\n";
      ss << "It sucks when yangle isn't available\n";
      ss << "10 reasons why yangle is the best IT company\n";
      ss << "yangle rocks others suck\n";
      ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
      }
      { LOG_DURATION("Explore key words one thread");
      const auto stats = ExploreKeyWordsOneThread(key_words, ss);
      const map<string, int> expected = {
        {"yangle", 6*100000},
        {"rocks", 2*100000},
        {"sucks", 1*100000}
      };
      ASSERT_EQUAL(stats.word_frequences, expected);
      }
    }

    {
      const set<string> key_words = {"yangle", "rocks", "sucks", "all"};
      stringstream ss;
      for (int i = 0; i < 100000; ++i) {
      ss << "this new yangle service really rocks\n";
      ss << "It sucks when yangle isn't available\n";
      ss << "10 reasons why yangle is the best IT company\n";
      ss << "yangle rocks others suck\n";
      ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
      }
      { LOG_DURATION("Explore key words multi threads");
      const auto stats = ExploreKeyWords(key_words, ss);
      const map<string, int> expected = {
        {"yangle", 6*100000},
        {"rocks", 2*100000},
        {"sucks", 1*100000}
      };
      ASSERT_EQUAL(stats.word_frequences, expected);
      }
    }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
