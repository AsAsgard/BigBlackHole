#include <string>
#include <list>
#include <vector>
#include "test_runner.h"
using namespace std;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() : cursor(text.begin()) {}
  void Left() { if (cursor != text.begin()) cursor = prev(cursor);}
  void Right() { if (cursor != text.end()) cursor = next(cursor);}
  void Insert(char token) { text.insert(cursor, token);}

  void Cut(size_t tokens = 1)
  {
      auto dist = distance(cursor, text.end());
      if (dist > tokens) {
        auto it = next(cursor, tokens);
        buffer = vector<char>(cursor, it);
        text.erase(cursor, it);
        cursor = it;
      } else {
        buffer = vector<char>(cursor, text.end());
        text.erase(cursor, text.end());
        cursor = text.end();
      }
  }

  void Copy(size_t tokens = 1)
  {
      auto dist = distance(cursor, text.end());
      if (dist > tokens) {
        buffer = vector<char>(cursor, next(cursor, tokens));
      } else {
        buffer = vector<char>(cursor, text.end());
      }
  }

  void Paste() { text.insert(cursor, buffer.begin(), buffer.end());}

  string GetText() const
  {
      string result = "";
      for (const auto& ch : text)
      {
          result.push_back(ch);
      }
      return result;
  }

private:
  list<char> text;
  list<char>::iterator cursor;
  vector<char> buffer;
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  RUN_TEST(tr, TestEditing);
  return 0;
}
