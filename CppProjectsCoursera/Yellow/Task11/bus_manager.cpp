#include "bus_manager.h"

using namespace std;

void BusManager::AddBus(const string& bus, const vector<string>& stops) {
  // Реализуйте этот метод
  buses_to_stops[bus] = stops;
  for (const auto& stop: stops)
  {
      stops_to_buses[stop].emplace_back(bus);
  }
}

BusesForStopResponse BusManager::GetBusesForStop(const string& stop) const {
  // Реализуйте этот метод
    BusesForStopResponse bs;
    bs.stops_to_buses = stops_to_buses;
    bs.stop = stop;
    return bs;
}

StopsForBusResponse BusManager::GetStopsForBus(const string& bus) const {
  // Реализуйте этот метод
  StopsForBusResponse sfb;
  sfb.bus = bus;
  sfb.buses_to_stops = buses_to_stops;
  sfb.stops_to_buses = stops_to_buses;
  return sfb;
}

AllBusesResponse BusManager::GetAllBuses() const {
  // Реализуйте этот метод
  return {buses_to_stops};
}
