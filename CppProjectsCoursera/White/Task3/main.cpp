#include <iostream>
#include <set>
#include <map>

using namespace std;

int main()
{
    int N = 0;
    set<pair<string,string>> Sinonims;
    map<string,int> SinonimsCount;
    cin >> N;
    for (int i = 0; i < N; ++i)
    {
        string str = "";
        cin >> str;
        if (str == "ADD")
        {
            string sin1 = "", sin2 = "";
            cin >> sin1 >> sin2;
            pair<string,string> newSinonims;
            if (sin1 > sin2) {newSinonims.first = sin2; newSinonims.second = sin1;}
            else {newSinonims.first = sin1; newSinonims.second = sin2;}
            if (Sinonims.count(newSinonims) == 0)
            {
                ++SinonimsCount[sin1];
                ++SinonimsCount[sin2];
                Sinonims.insert(newSinonims);
            }
        }
        if (str == "COUNT")
        {
            string sinonim;
            cin >> sinonim;
            cout << SinonimsCount[sinonim] << endl;
        }
        if (str == "CHECK")
        {
            string sin1 = "", sin2 = "";
            cin >> sin1 >> sin2;
            pair<string,string> newSinonims;
            if (sin1 > sin2) {newSinonims.first = sin2; newSinonims.second = sin1;}
            else {newSinonims.first = sin1; newSinonims.second = sin2;}
            if (Sinonims.count(newSinonims) != 0)
            {
                cout << "YES" << endl;
            } else {
                cout << "NO" << endl;
            }
        }
    }
    return 0;
}
