#ifndef RESPONSES_H
#define RESPONSES_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

struct BusesForStopResponse {
  // Наполните полями эту структуру
  std::map<std::string, std::vector<std::string>> stops_to_buses;
  std::string stop;
};

std::ostream& operator << (std::ostream& os, const BusesForStopResponse& r);

struct StopsForBusResponse {
  // Наполните полями эту структуру
  std::map<std::string, std::vector<std::string>> buses_to_stops, stops_to_buses;
  std::string bus;
};

std::ostream& operator << (std::ostream& os, const StopsForBusResponse& r);

struct AllBusesResponse {
  // Наполните полями эту структуру
    std::map<std::string, std::vector<std::string>> buses_to_stops;
};

std::ostream& operator << (std::ostream& os, const AllBusesResponse& r);


#endif // RESPONSES_H
