#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N = 0;
    long double sum = 0;
    cin >> N;
    vector<long double> v(N);
    for (auto &val : v)
    {
        cin >> val;
        sum += val;
    } sum /= N;

    vector<int> vb;
    for (int i = 0; i < N; ++i)
    {
        if (v[i] > sum) vb.emplace_back(i);
    }
    cout << vb.size() << endl;
    for (auto val : vb)
    {
        cout << val << " ";
    }
    return 0;
}
