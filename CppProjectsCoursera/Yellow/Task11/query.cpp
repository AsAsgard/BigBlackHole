#include "query.h"

using namespace std;

istream& operator >> (istream& is, Query& q) {
  // Реализуйте эту функцию
  string operation_code("");
  is >> operation_code;
  if (operation_code != "NEW_BUS" && operation_code != "BUSES_FOR_STOP" && operation_code != "STOPS_FOR_BUS" && operation_code != "ALL_BUSES")
  {
      q.type = QueryType::BadCommand;
      return is;
  }
  if (operation_code == "NEW_BUS") q.type = QueryType::NewBus;
  if (operation_code == "BUSES_FOR_STOP") q.type = QueryType::BusesForStop;
  if (operation_code == "STOPS_FOR_BUS") q.type = QueryType::StopsForBus;
  if (operation_code == "ALL_BUSES") q.type = QueryType::AllBuses;
  switch (q.type) {
    case QueryType::NewBus:
      {
        is >> q.bus;
        int stops_count = 0;
        is >> stops_count;
        q.stops.resize(stops_count);
        for (auto& stop_name : q.stops)
        {
            is >> stop_name;
        }
      }
      break;
    case QueryType::BusesForStop:
      is >> q.stop;
      break;
    case QueryType::StopsForBus:
      is >> q.bus;
      break;
    case QueryType::AllBuses:
      break;
  }
  return is;
}
