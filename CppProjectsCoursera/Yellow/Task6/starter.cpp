#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses,
  BadCommand
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

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

struct BusesForStopResponse {
  // Наполните полями эту структуру
  map<string, vector<string>> stops_to_buses;
  string stop;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
  // Реализуйте эту функцию
  if (r.stops_to_buses.count(r.stop) == 0)
  {
      os << "No stop";
  } else {
      for (const auto& bus : r.stops_to_buses.at(r.stop))
      {
          os << bus << " ";
      }
  }
  return os;
}

struct StopsForBusResponse {
  // Наполните полями эту структуру
  map<string, vector<string>> buses_to_stops, stops_to_buses;
  string bus;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
  // Реализуйте эту функцию
  if (r.buses_to_stops.count(r.bus) == 0) {
    os << "No bus";
  } else {
      for (auto it = r.buses_to_stops.at(r.bus).begin(); it != r.buses_to_stops.at(r.bus).end(); ++it)
      {
          if (it != r.buses_to_stops.at(r.bus).begin()) os << endl;
          os << "Stop " << *it << ": ";
          if (r.stops_to_buses.at(*it).size() == 1) {
            os << "no interchange";
          } else {
            for (const string& other_bus : r.stops_to_buses.at(*it)) {
              if (r.bus != other_bus) {
                os << other_bus << " ";
              }
            }
          }
      }
  }
  return os;
}

struct AllBusesResponse {
  // Наполните полями эту структуру
    map<string, vector<string>> buses_to_stops;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
  // Реализуйте эту функцию
  if (r.buses_to_stops.empty()) {
    os << "No buses";
  } else {
    for (auto it = r.buses_to_stops.begin(); it != r.buses_to_stops.end(); ++it) {
      if (it != r.buses_to_stops.begin()) os << endl;
      os << "Bus " << it->first << ": ";
      for (const string& stop : it->second) {
        os << stop << " ";
      }
    }
  }
  return os;
}

class BusManager {
public:
  void AddBus(const string& bus, const vector<string>& stops) {
    // Реализуйте этот метод
    buses_to_stops[bus] = stops;
    for (const auto& stop: stops)
    {
        stops_to_buses[stop].emplace_back(bus);
    }
  }

  BusesForStopResponse GetBusesForStop(const string& stop) const {
    // Реализуйте этот метод
      BusesForStopResponse bs;
      bs.stops_to_buses = stops_to_buses;
      bs.stop = stop;
      return bs;
  }

  StopsForBusResponse GetStopsForBus(const string& bus) const {
    // Реализуйте этот метод
    StopsForBusResponse sfb;
    sfb.bus = bus;
    sfb.buses_to_stops = buses_to_stops;
    sfb.stops_to_buses = stops_to_buses;
    return sfb;
  }

  AllBusesResponse GetAllBuses() const {
    // Реализуйте этот метод
    return {buses_to_stops};
  }
private:
  map<string, vector<string>> buses_to_stops, stops_to_buses;
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus) << endl;
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses() << endl;
      break;
    }
  }

  return 0;
}
