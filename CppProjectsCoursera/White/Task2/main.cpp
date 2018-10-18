#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main()
{
    int N = 0;
    map<string,vector<string>> BusMap;
    vector<string> BusOrder;
    cin >> N;
    for (int i = 0; i < N; ++i)
    {
        string str = "";
        cin >> str;
        if (str == "NEW_BUS")
        {
            string bus = "";
            int stop_count = 0;
            cin >> bus >> stop_count;
            BusOrder.emplace_back(bus);
            BusMap[bus].assign(stop_count,"");
            for (auto & stopname : BusMap[bus])
            {
                cin >> stopname;
            }
        }
        if (str == "BUSES_FOR_STOP")
        {
            string stop = "";
            cin >> stop;
            bool StopExists = false;
            for (auto it = BusOrder.begin(); it != BusOrder.end(); it++)
            {
                if (count(BusMap[*it].begin(), BusMap[*it].end(), stop) > 0)
                {
                    cout << *it << " ";
                    StopExists = true;
                }
            }
            if (!StopExists) cout << "No stop";
            cout << endl;
        }
        if (str == "STOPS_FOR_BUS")
        {
            string bus = "";
            cin >> bus;
            if (BusMap.count(bus) == 0) {cout << "No bus" << endl; continue;}
            for (const auto & StopName : BusMap[bus])
            {
                cout << "Stop " << StopName << ": ";
                bool InterchangeExists = false;
                for (const auto & BusName : BusOrder)
                {
                    if (BusName == bus) continue;
                    if (count(BusMap[BusName].begin(), BusMap[BusName].end(), StopName) > 0)
                    {
                        cout << BusName << " ";
                        InterchangeExists = true;
                    }
                }
                if (!InterchangeExists) cout << "no interchange";
                cout << endl;
            }
        }
        if (str == "ALL_BUSES")
        {
            if (BusMap.empty()) {cout << "No buses" << endl; continue;}
            for (const auto & BusPair : BusMap)
            {
                cout << "Bus " << BusPair.first << ": ";
                for (const auto & StopName : BusPair.second)
                {
                    cout << StopName << " ";
                }
                cout << endl;
            }
        }
        cin.clear();
    }
    return 0;
}
