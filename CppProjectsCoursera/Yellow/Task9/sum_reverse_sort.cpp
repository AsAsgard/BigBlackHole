#include "sum_reverse_sort.h"

#include <algorithm>

int Sum(int x, int y)
{
    return x+y;
}


string Reverse(string s)
{
    string reversed = "";
    for_each(s.rbegin(), s.rend(), [&reversed] (const char& ch)
    {
        reversed.push_back(ch);
    });
    return reversed;
}


void Sort(vector<int>& nums)
{
    sort(nums.begin(), nums.end());
}
