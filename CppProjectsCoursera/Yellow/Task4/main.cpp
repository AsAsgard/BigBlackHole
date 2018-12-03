#include <iostream>
#include <vector>
#include <map>

using namespace std;

template<typename T> T Sqr(const T& value);
template<typename T> vector<T> Sqr(const vector<T>& v);
template<typename T1, typename T2> map<T1, T2> Sqr(const map<T1, T2>& m);
template<typename T1, typename T2> pair<T1, T2> Sqr(const pair<T1, T2>& p);

template<typename T>
T Sqr(const T& value)
{
    return value*value;
}

template<typename T>
vector<T> Sqr(const vector<T>& v)
{
    vector<T> newV;
    for (const auto& value : v)
    {
        newV.emplace_back(Sqr(value));
    }
    return newV;
}

template<typename T1, typename T2>
map<T1, T2> Sqr(const map<T1, T2>& m)
{
    map<T1,T2> newM;
    for (const auto& pr : m)
    {
        newM[pr.first] = Sqr(pr.second);
    }
    return newM;
}

template<typename T1, typename T2>
pair<T1, T2> Sqr(const pair<T1, T2>& p)
{
    pair<T1, T2> newP;
    newP.first = Sqr(p.first);
    newP.second = Sqr(p.second);
    return newP;
}


int main()
{
    // Пример вызова функции
    vector<int> v = {1, 2, 3};
    cout << "vector:";
    for (int x : Sqr(v)) {
      cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
      {4, {2, 2}},
      {7, {4, 3}}
    };
    cout << "map of pairs:" << endl;
    for (const auto& x : Sqr(map_of_pairs)) {
      cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
    return 0;
}
