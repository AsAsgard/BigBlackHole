#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    const int MAX_ATHLETES_NUMBER = 100'000;

    int query_count;
    int athlete_number, prev_number;
    cin >> query_count;
    list<int> athletes_order;
    vector<list<int>::iterator> iterators(MAX_ATHLETES_NUMBER + 1, athletes_order.end());

    for (int i = 0; i < query_count; ++i)
    {
        cin >> athlete_number >> prev_number;

        iterators[athlete_number] = athletes_order.emplace(iterators[prev_number], athlete_number);
    }
    for (const auto& el : athletes_order)
        cout << el << " ";
    return 0;
}
