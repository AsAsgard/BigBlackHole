#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
       os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

bool IsPalindrom(const string& str) {
    if (str.size() == 0) return true;
    for (int i =0; i < str.size()/2 + 1; ++i)
    {
        if (str[i] != str[str.size()-1-i]) return false;
    }
    return true;
}

void ZeroString(void)
{
    Assert(IsPalindrom(""),"Zero string");
}

void OneCharString(void)
{
    Assert(IsPalindrom(" "), "Probel");
    Assert(IsPalindrom("//"), "// String");
    Assert(IsPalindrom("b"), "One char");
}

void SomeString(void)
{
    Assert(IsPalindrom("madam"),"madam");
    Assert(!IsPalindrom("amdam"),"amdam");
    Assert(!IsPalindrom("mmadam"),"mmadam");
    Assert(IsPalindrom("mm"),"mm");
    Assert(IsPalindrom("m m"),"m m");
    Assert(!IsPalindrom("md"),"md");
    Assert(!IsPalindrom("m d"),"m d");
    Assert(IsPalindrom("m//m"),"m//m");
    Assert(!IsPalindrom("m//d"),"m//d");
    Assert(!IsPalindrom("madamd"),"madamd");
    Assert(IsPalindrom("mam"),"mam");
    Assert(IsPalindrom("mddm"),"mddm");
    Assert(!IsPalindrom("mdbm"),"mdbm");
    Assert(IsPalindrom("wasitacaroracatisaw"),"wasitacaroracatisaw");
    Assert(!IsPalindrom("md bm"),"md bm");
    Assert(IsPalindrom("md dm"),"md dm");
    Assert(!IsPalindrom("md  bm"),"md  bm");
    Assert(IsPalindrom("md  dm"),"md  dm");
    Assert(!IsPalindrom("mdd m"),"mdd m");
    Assert(!IsPalindrom(" mddm")," mddm");
    Assert(!IsPalindrom("//mddm"),"//mddm");

}

int main() {
  TestRunner runner;
  // добавьте сюда свои тесты
  runner.RunTest(ZeroString,"ZeroString");
  runner.RunTest(OneCharString,"OneCharString");
  runner.RunTest(SomeString,"SomeString");
  return 0;
}
