#include <iostream>
#include <algorithm>
#include <list>

using namespace std;

int main()
{
    int N = 0;
    cin >> N;
    list<int> ls;
    for (int i = N; i!= 0; --i) { ls.emplace_back(i);}
    do {
        for (auto it = ls.begin(); it != ls.end(); ++it)
        {
            if (it != ls.begin()) cout << " ";
            cout << *it;
        }
        cout << endl;
    } while(prev_permutation(ls.begin(), ls.end()));
    return 0;
}
