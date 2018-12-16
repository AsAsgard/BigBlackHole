#ifndef DATABASE_H
#define DATABASE_H

#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "date.h"

template<typename T, typename U>
std::ostream& operator<<(std::ostream& out, std::pair<T, U> p)
{
    out << p.first << " " << p.second;
    return out;
}

class Database {
public:
  void Add(const Date& date, const std::string& event);

  template<typename PredFunc> int RemoveIf(PredFunc predicate)
  {
      int DeletedCount = 0;
      std::for_each(EventsMap.begin(),
                    EventsMap.end(),
                    [&predicate, &DeletedCount]
                    (std::pair<const Date,
                               std::pair< std::vector<std::set<std::string>::iterator>,
                                          std::set<std::string>> >
                                          & EventsForDate) mutable
      {
            DeletedCount += EventsForDate.second.second.size();
            auto it = std::stable_partition(EventsForDate.second.first.begin(),
                                            EventsForDate.second.first.end(),
                                            [&EventsForDate, &predicate] (const std::set<std::string>::iterator& element)
            {
                return !predicate(EventsForDate.first, *element);
            });
            for (auto sit = it; sit != EventsForDate.second.first.end(); ++sit)
            {
                EventsForDate.second.second.erase(*sit);
            }
            EventsForDate.second.first.erase(it, EventsForDate.second.first.end());
            DeletedCount -= EventsForDate.second.second.size();
      });
      for (auto it = EventsMap.begin(); it != EventsMap.end();)
      {
          if (it->second.second.empty()) {
              EventsMap.erase(it++);
          } else {
              ++it;
          }
      }
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
