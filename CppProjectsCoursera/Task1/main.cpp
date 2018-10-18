#include <iostream>
#include <map>

using namespace std;

int main()
{
    int N = 0;
    map<string, string> CountryMap;
    cin >> N;
    for (int i = 0; i < N; ++i)
    {
        string str = "";
        cin >> str;
        if (str == "CHANGE_CAPITAL")
        {
            string str1 = "", str2 = "";
            cin >> str1 >> str2;
            if (CountryMap.count(str1) == 0) {cout << "Introduce new country " << str1 <<" with capital " << str2 << endl; CountryMap[str1] = str2; continue;}
            else if (CountryMap[str1] == str2) {cout << "Country " << str1 << " hasn't changed its capital" << endl; continue;}
            cout << "Country " << str1 << " has changed its capital from " << CountryMap[str1] << " to " << str2 << endl;
            CountryMap[str1] = str2;
        }
        if (str == "RENAME")
        {
            string str1 = "", str2 = "";
            cin >> str1 >> str2;
            if (CountryMap.count(str1) == 0 || str1 == str2 || CountryMap.count(str2) == 1) {cout << "Incorrect rename, skip" << endl; continue;}
            CountryMap[str2] = CountryMap[str1];
            CountryMap.erase(str1);
            cout << "Country " << str1 << " with capital " << CountryMap[str2] << " has been renamed to " << str2 << endl;
        }
        if (str == "ABOUT")
        {
            string str1 = "";
            cin >> str1;
            if (CountryMap.count(str1) == 0)
            {
                cout << "Country " << str1 << " doesn't exist" << endl;
            } else {
                cout << "Country " << str1 << " has capital " << CountryMap[str1] << endl;
            }
        }
        if (str == "DUMP")
        {
            if (CountryMap.empty()) {cout << "There are no countries in the world" << endl; continue;}
            for (const auto & CountryPair : CountryMap)
            {
                cout << CountryPair.first << "/" << CountryPair.second << " ";
            }
            cout << endl;
        }
        cin.clear();
    }
    return 0;
}
