#include "database.h"
#include <algorithm>
#include <memory>

using namespace std;

void Database::Add(const Date& date, const string& event)
{
    auto insertion = EventsMap[date].second.insert(event);
    if (insertion.second) EventsMap[date].first.emplace_back(insertion.first);
}

void Database::Print(std::ostream& out) const
{
    for(auto mit = EventsMap.begin(); mit != EventsMap.end(); ++mit)
    {
        for(auto vit = mit->second.first.begin(); vit != mit->second.first.end(); ++vit)
        {
            out << mit->first << ' ' << **vit << endl;
        }
    }
}

pair<Date, string> Database::Last(const Date& date) const
{
    auto up_bound = EventsMap.upper_bound(date);
    if (up_bound == EventsMap.begin()) throw invalid_argument("");
    auto elem = prev(up_bound);
    return make_pair(elem->first, **elem->second.first.rbegin());
}
