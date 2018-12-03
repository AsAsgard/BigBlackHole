#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
    if (next(range_begin) == range_end || range_begin == range_end) return;
    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    auto border1 = elements.begin() + elements.size()/3;
    auto border2 = elements.begin() + 2*elements.size()/3;
    MergeSort(elements.begin(),border1);
    MergeSort(border1,border2);
    MergeSort(border2, elements.end());
    vector<typename RandomIt::value_type> tmp;
    merge(elements.begin(),border1, border1, border2, back_inserter(tmp));
    merge(tmp.begin(),tmp.end(), border2, elements.end(), range_begin);
}
/*
int main() {
  vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    cout << x << " ";
  }
  cout << endl;
  return 0;
}*/
