#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    if (next(range_begin) == range_end || range_begin == range_end) return;
    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    auto border = elements.begin() + elements.size()/2;
    MergeSort(elements.begin(),border);
    MergeSort(border, elements.end());
    merge(elements.begin(),border, border, elements.end(), range_begin);
}
/*
int main() {
  vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    cout << x << " ";
  }
  cout << endl;
  return 0;
}*/
