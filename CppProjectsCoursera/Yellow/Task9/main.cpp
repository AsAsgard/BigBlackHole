#include "sum_reverse_sort.h"
#include <iostream>

using namespace std;

int main()
{
    cout << Sum(4,-5) << endl;
    cout << Reverse("marinos") << endl;
    cout << Reverse("") << endl;
    cout << Reverse(" ") << endl;
    cout << Reverse("m") << endl;
    vector<int> v = {3,8,12,-3,5,14, 14, 17, 8, -17, 3};
    Sort(v);
    for (const auto& i : v)
    {
        cout << i << " ";
    }
    return 0;
}
