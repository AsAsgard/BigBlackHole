#include "test_runner.h"

#include <vector>
using namespace std;

// Реализуйте здесь шаблонный класс Table
template<typename T>
class Table {
    vector<vector<T>> Data;
public:
    Table(size_t rows = 0, size_t cols = 0)
    {
        if (rows != 0 && cols != 0)
        {
            Data.resize(rows);
            for (auto& v : Data) v.resize(cols);
        }
    }

    vector<T>& operator[](const size_t& row) {return Data[row];}
    const vector<T>& operator[](const size_t& row) const {return Data[row];}

    void Resize(size_t rows, size_t cols) {Data.resize(rows); for (auto& v : Data) v.resize(cols);}

    pair<size_t, size_t> Size() const
    {
        if (Data.size() != 0)
            return {Data.size(), Data[0].size()};
        else
            return {0, 0};
    }
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
