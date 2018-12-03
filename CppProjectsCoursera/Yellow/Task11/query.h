#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <vector>
#include <iostream>

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses,
  BadCommand
};

struct Query {
  QueryType type;
  std::string bus;
  std::string stop;
  std::vector<std::string> stops;
};

std::istream& operator >> (std::istream& is, Query& q);

#endif // QUERY_H
