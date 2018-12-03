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

/*
// УДАЛИТЬ!!!!!!!!!!!
class Rational {
public:
    // Вы можете вставлять сюда различные реализации функции,
    // чтобы проверить, что ваши тесты пропускают корректный код
    // и ловят некорректный

  Rational() : numer(0), denom(1) {}
  Rational(int numerator, int denominator) {
      this->numer = numerator;
      if (denominator < 0)
      {
          this->numer *= -1;
          denominator *= -1;
      }
      this->denom = denominator;
      this->RationalReduction();
  }

void RationalReduction()
  {
      int a = abs(this->numer);
      int b = abs(this->denom);
      while (b!=0)
      {
          int c = a % b;
          a = b;
          b = c;
      }
      this->numer /= a;
      this->denom /= a;
  }

  int Numerator() const { return numer;}

  int Denominator() const { return denom;}

private:
  int numer, denom;
};
*/

void TestDefaultConstructor(void)
{
    Rational y;
    AssertEqual(y.Numerator(), 0, "Without parentheses numer");
    AssertEqual(y.Denominator(), 1, "Without parentheses denom");
}

void TestFullConstructor(void)
{
    Rational y(5, 4);
    Rational x(3, 4);
    Rational z(1,1000000);
    AssertEqual(y.Numerator(), 5, "Y numerator full constructor");
    AssertEqual(y.Denominator(), 4, "Y denominator full constructor");
    AssertEqual(x.Numerator(), 3, "X numerator full constructor");
    AssertEqual(x.Denominator(), 4, "X denominator full constructor");
    AssertEqual(z.Numerator(), 1, "Z numerator full constructor");
    AssertEqual(z.Denominator(), 1000000, "Z denominator full constructor");
}

void TestReduction(void)
{
    Rational y(6, 4);
    Rational x(2, 4);
    Rational z(6,1000000);
    AssertEqual(y.Numerator(), 3, "Y numerator reduction");
    AssertEqual(y.Denominator(), 2, "Y denominator reduction");
    AssertEqual(x.Numerator(), 1, "X numerator reduction");
    AssertEqual(x.Denominator(), 2, "X denominator reduction");
    AssertEqual(z.Numerator(), 3, "Z numerator reduction");
    AssertEqual(z.Denominator(), 500000, "Z denominator reduction");
}

void TestZeroNumerator(void)
{
    Rational y(0, 4);
    Rational x(0, 1212121212);
    Rational z(0,-10000);
    AssertEqual(y.Numerator(), 0, "Y numerator reduction");
    AssertEqual(y.Denominator(), 1, "Y denominator reduction");
    AssertEqual(x.Numerator(), 0, "X numerator reduction");
    AssertEqual(x.Denominator(), 1, "X denominator reduction");
    AssertEqual(z.Numerator(), 0, "Z numerator reduction");
    AssertEqual(z.Denominator(), 1, "Z denominator reduction");
}

void TestSign(void)
{
    Rational y(-6, -4);
    Rational x(-2, 4);
    Rational z(6,-1000000);
    AssertEqual(y.Numerator(), 3, "Y numerator reduction");
    AssertEqual(y.Denominator(), 2, "Y denominator reduction");
    AssertEqual(x.Numerator(), -1, "X numerator reduction");
    AssertEqual(x.Denominator(), 2, "X denominator reduction");
    AssertEqual(z.Numerator(), -3, "Z numerator reduction");
    AssertEqual(z.Denominator(), 500000, "Z denominator reduction");
}


int main() {
  TestRunner runner;
  // добавьте сюда свои тесты
  runner.RunTest( TestDefaultConstructor ,"TestDefaultConstructor");
  runner.RunTest( TestFullConstructor ,"TestFullConstructor");
  runner.RunTest( TestReduction ,"TestReduction");
  runner.RunTest( TestZeroNumerator ,"TestZeroNumerator");
  runner.RunTest( TestSign ,"TestSign");
  return 0;
}
