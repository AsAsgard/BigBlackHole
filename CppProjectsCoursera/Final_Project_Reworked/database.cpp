#include "database.h"
#include <algorithm>
#include <memory>

using namespace std;

void Database::Add(const Date& date, const string& event)
{
     auto insertion = EventsMap[date].second.insert(event);
     if (insertion.second) EventsMap[date].first.emplace_back(insertion.first);
}

bool Database::DeleteEvent(const Date& date, const string& event)
{
    if(EventsMap.count(date))
    {
        if(EventsMap.at(date).second.count(event))
        {
            EventsMap.at(date).second.erase(event);
            if(EventsMap.at(date).second.empty()) EventsMap.erase(date);
            return true;
        }
    }
    return false;
}

int Database::DeleteDate(const Date& date)
{
    if(EventsMap.count(date))
    {
        int DeletedEventsCount = static_cast<int>(EventsMap.at(date).second.size());
        EventsMap.erase(date);
        return DeletedEventsCount;
    }
    return 0;
}

set<string> Database::Find(const Date& date) const
{
    if(EventsMap.count(date)) return EventsMap.at(date).second;
    return set<string>();
}

void Database::Print(std::ostream& out) const
{
    for(auto mit = EventsMap.begin(); mit != EventsMap.end(); mit++)
    {
        for(auto vit = mit->second.first.begin(); vit != mit->second.first.end(); vit++)
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
