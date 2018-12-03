#include "responses.h"

using namespace std;

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
