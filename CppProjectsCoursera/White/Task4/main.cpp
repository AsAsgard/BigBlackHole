#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

template<typename T>
void PrintVectorInt(const vector<T> & TVector)
{
    for (const auto & elem : TVector)
    {
        cout << elem << " ";
    }
    cout << endl;
}

void TolowerString(string& str)
{
    for (auto & character: str)
    {
        character = tolower(character);
    }
}

int main()
{
    int N = 0;
    cin >> N;
    vector<string> StringValues(N);
    for (auto & str : StringValues)
    {
        cin >> str;
    }
    sort(StringValues.begin(), StringValues.end(), [] (string str1, string str2)
    {
        TolowerString(str1);
        TolowerString(str2);
        return str1 < str2;
    });

    PrintVectorInt(StringValues);
    return 0;
}
