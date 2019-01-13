#include "test_runner.h"
#include <string>
#include <deque>
#include <map>
#include <algorithm>


using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target)
  {
    auto sourceIt = data_views_sources.find(source);
    auto targetIt = data_views_targets.find(target);
    if (sourceIt != data_views_sources.end() && targetIt != data_views_targets.end())
    {
        data_views_sources[sourceIt->first] = targetIt->first;
        data_views_targets[targetIt->first] = sourceIt->first;
    } else if (sourceIt != data_views_sources.end()) {
        const string_view target_str = data.emplace_back(target);
        data_views_sources[sourceIt->first] = target_str;
        data_views_targets[target_str] = sourceIt->first;
    } else if (targetIt != data_views_targets.end()) {
        const string_view source_str = data.emplace_back(source);
        data_views_sources[source_str] = targetIt->first;
        data_views_targets[targetIt->first] = source_str;
    } else {
        const string_view source_str = data.emplace_back(source);
        const string_view target_str = data.emplace_back(target);
        data_views_sources[source_str] = target_str;
        data_views_targets[target_str] = source_str;
    }
  }

  string_view TranslateForward(string_view source) const
  {
      if (data_views_sources.count(source) > 0)
         return data_views_sources.at(source);
      else
         return string_view();
  }

  string_view TranslateBackward(string_view target) const
  {
      if (data_views_targets.count(target) > 0)
          return data_views_targets.at(target);
      else
          return string_view();
  }

private:
  deque<string> data;
  map<string_view, string_view> data_views_sources;
  map<string_view, string_view> data_views_targets;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");

  translator.Add(string("okoshko"), string("window"));
  translator.Add(string("stol"), string("board"));

  ASSERT_EQUAL(translator.TranslateBackward("window"), "okoshko");
  ASSERT_EQUAL(translator.TranslateBackward("board"), "stol");
  ASSERT_EQUAL(translator.TranslateForward("okoshko"), "window");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
