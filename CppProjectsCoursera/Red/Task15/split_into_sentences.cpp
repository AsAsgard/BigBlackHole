#include "test_runner.h"

#include <vector>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
// Напишите реализацию функции, не копируя объекты типа Token
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  vector<Sentence<Token>> result;
  bool next_sent = true;
  for (auto& tok : tokens)
  {
      if ((next_sent == true && !tok.IsEndSentencePunctuation()) || result.empty()) {
          result.emplace_back();
          next_sent = false;
      }
      if (tok.IsEndSentencePunctuation()) next_sent = true;
      result.back().push_back(move(tok));
  }
  return result;
}


struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

struct TestToken1 {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }

  TestToken1(string data_, bool is_end = false) : data(data_), is_end_sentence_punctuation(is_end)
  {
  }
  TestToken1(const TestToken1& other) = delete;
/*      : data(other.data)
      , is_end_sentence_punctuation(other.is_end_sentence_punctuation)
  {
  }*/
  TestToken1(TestToken1&& other)
      : data(move(other.data))
      , is_end_sentence_punctuation(other.is_end_sentence_punctuation)
  {
  }
  TestToken1& operator=(const TestToken1& other) = delete;
/*  {
      data = other.data;
      is_end_sentence_punctuation = other.is_end_sentence_punctuation;
      return *this;
  }*/
  TestToken1& operator=(TestToken1&& other)
  {
      data = move(other.data);
      is_end_sentence_punctuation = move(other.is_end_sentence_punctuation);
      return *this;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    vector<TestToken1> v1;
    v1.emplace_back("Split");
    v1.emplace_back("into");
    v1.emplace_back("sentences");
    v1.emplace_back("!", true);
    v1.emplace_back("!", true);
    v1.emplace_back("Without");
    v1.emplace_back("copies");
    v1.emplace_back(".", true);
    auto result = SplitIntoSentences(move(v1));

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!"}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({ {"!", true}, {"!", true}, {"!", true}, {"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}, {".", true}, {".", true}})),
    vector<Sentence<TestToken>>({
        {{"!", true}, {"!", true}, {"!", true}},
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}, {".", true}, {".", true}},
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"!", true}, {"!", true}, {"!", true}, {"!", true}})),
    vector<Sentence<TestToken>>({
        {{"!", true}, {"!", true}, {"!", true}, {"!", true}}
    })
  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  return 0;
}
