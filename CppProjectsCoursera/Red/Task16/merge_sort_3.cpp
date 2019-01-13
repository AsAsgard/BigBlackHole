#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    if (next(range_begin) == range_end || range_begin == range_end) return;
    vector<typename RandomIt::value_type> elements(make_move_iterator(range_begin), make_move_iterator(range_end));
    auto border1 = elements.begin() + elements.size()/3;
    auto border2 = elements.begin() + 2*elements.size()/3;
    MergeSort(elements.begin(),border1);
    MergeSort(border1,border2);
    MergeSort(border2, elements.end());
    vector<typename RandomIt::value_type> tmp;
    merge(make_move_iterator(elements.begin()),make_move_iterator(border1), make_move_iterator(border1), make_move_iterator(border2), back_inserter(tmp));
    merge(make_move_iterator(tmp.begin()),make_move_iterator(tmp.end()), make_move_iterator(border2), make_move_iterator(elements.end()), range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
