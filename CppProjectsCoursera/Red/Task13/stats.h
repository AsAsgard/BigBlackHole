#pragma once

#include "http_request.h"

#include <string_view>
#include <array>
#include <map>
using namespace std;

class Stats {
public:
  static const array<string, 5> MethodNames;
  static const array<string, 6> UriNames;

  Stats();
  void AddMethod(string_view method);
  void AddUri(string_view uri);
  const map<string_view, int>& GetMethodStats() const;
  const map<string_view, int>& GetUriStats() const;

private:
  map<string_view, int> MethodStats;
  map<string_view, int> UriStats;
};

HttpRequest ParseRequest(string_view line);
