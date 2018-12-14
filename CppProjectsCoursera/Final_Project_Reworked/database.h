#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "date.h"

class Database {
public:
  void Add(const Date& date, const std::string& event);

  template<typename PredFunc> int RemoveIf(PredFunc predicate)
  {
      int DeletedCount = 0;
      Database RemainingEvents;
      std::for_each(EventsMap.begin(),
                    EventsMap.end(),
                    [&predicate, &DeletedCount, &RemainingEvents]
                    (std::pair<const Date,
                     std::pair<std::vector<std::set<std::string>::iterator>, std::set<std::string>>>& EventsForDate) mutable
      {
            std::for_each(EventsForDate.second.first.begin(),
                          EventsForDate.second.first.end(),
                          [&predicate, &EventsForDate, &RemainingEvents] (const std::set<std::string>::iterator& event)
            {
                if (!predicate(EventsForDate.first, *event)) RemainingEvents.Add(EventsForDate.first, *event);
            });
            if (RemainingEvents.EventsMap.count(EventsForDate.first))
            {
                DeletedCount += EventsForDate.second.second.size() - RemainingEvents.EventsMap.at(EventsForDate.first).second.size();
            } else {
                DeletedCount += EventsForDate.second.second.size();
            }
      });
      EventsMap = RemainingEvents.EventsMap;
      return DeletedCount;
  }

  template<typename PredFunc> std::vector<std::pair<Date, std::string>> FindIf(PredFunc predicate) const
  {
      std::vector<std::pair<Date, std::string>> Result;
      std::for_each(EventsMap.begin(),
                    EventsMap.end(),
                    [&predicate, &Result]
                    (const std::pair<const Date,
                     std::pair<std::vector<std::set<std::string>::iterator>, std::set<std::string>>>& EventsForDate) mutable
      {
            std::for_each(EventsForDate.second.first.begin(),
                          EventsForDate.second.first.end(),
                          [&predicate, &EventsForDate, &Result] (const std::set<std::string>::iterator& event)
            {
                if (predicate(EventsForDate.first, *event)) Result.emplace_back(EventsForDate.first, *event);
            });
      });
      return Result;
  }

  void Print(std::ostream& out) const;
 
  std::pair<Date, std::string> Last(const Date& date) const;

private:
  std::map<Date, std::pair<std::vector<std::set<std::string>::iterator>, std::set<std::string>>> EventsMap;
};

#endif // DATABASE_H
