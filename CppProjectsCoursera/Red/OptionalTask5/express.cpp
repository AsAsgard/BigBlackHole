#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <thread>
#include "profile.h"

using namespace std;

class RouteManager {
public:
    void AddRoute(int start, int finish) {
      reachable_lists_[start].insert(finish);
      reachable_lists_[finish].insert(start);
      // O(logN)
    }
    int FindNearestFinish(int start, int finish) const {
      int result = abs(start - finish);
      if (reachable_lists_.count(start) < 1) {
          return result;
      }
      const set<int>& reachable_stations = reachable_lists_.at(start);
      if (!reachable_stations.empty()) {
        auto it = reachable_stations.lower_bound(finish);
        if (it != reachable_stations.end()) result = min(result, abs(finish - *it));
        if (it != reachable_stations.begin()) result = min(result, abs(finish - *prev(it)));
      }
      return result;
      // O(logN)
    }
private:
    map<int, set<int>> reachable_lists_;
};

/*
class RouteManagerVec {
public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].push_back(finish);
    reachable_lists_[finish].push_back(start);
    // O(1)
  }
  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.count(start) < 1) {
        return result;
    }
    const vector<int>& reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) {
      result = min(
          result,
          abs(finish - *min_element(
              begin(reachable_stations), end(reachable_stations),
              [finish](int lhs, int rhs) { return abs(lhs - finish) < abs(rhs - finish); }
          ))
      );
    }
    return result;
    // O(N)
  }
private:
  map<int, vector<int>> reachable_lists_;

};


int main() {
  RouteManagerSet SetRoutes;
  RouteManagerVec VecRoutes;

  int query_count;
  query_count = 2;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    if (query_id == 0) query_type = "ADD";
    else query_type = "GO";
    int start, finish;
    start = -3000000;
    if (query_type == "ADD") {
        int delta = 15000000;

        for (int i = (start - delta); i < (delta + start); i+=5) {
            SetRoutes.AddRoute(start, i);
            VecRoutes.AddRoute(start, i);
        }
        cout << "Add completed" << endl;

    } else if (query_type == "GO") {
      finish = 11343557;

      // GO for slow solution for only one time (vector)
      this_thread::sleep_for(chrono::seconds(1));
      {
        LOG_DURATION("Find in vector once                           ");
            VecRoutes.FindNearestFinish(start, finish);
      }

      // GO for fast solution for only one time (set)
      this_thread::sleep_for(chrono::seconds(1));
      {
        LOG_DURATION("Find in set once                              ");
            SetRoutes.FindNearestFinish(start, finish);
      }

      // GO for slow solution for many times (vector)
      this_thread::sleep_for(chrono::seconds(1));
      {
        LOG_DURATION("Find in vector for many times                 ");
        for (int i = 0; i < 1000; ++i)
            VecRoutes.FindNearestFinish(start, finish - i);
      }

      // GO for fast solution for many times (set)
      this_thread::sleep_for(chrono::seconds(1));
      {
        LOG_DURATION("Find in set for many times                    ");
        for (int i = 0; i < 1000; ++i)
            SetRoutes.FindNearestFinish(start, finish - i);
      }

      // GO for slow solution for many times (vector) with volatile
      this_thread::sleep_for(chrono::seconds(1));
      {
        LOG_DURATION("Find in vector for many times with volatile i ");
        for (volatile int i = 0; i < 1000; ++i)
            VecRoutes.FindNearestFinish(start, finish - i);
      }

      // GO for fast solution for many times (set) with volatile
      this_thread::sleep_for(chrono::seconds(1));
      {
        LOG_DURATION("Find in set for many times with volatile i    ");
        for (volatile int i = 0; i < 1000; ++i)
            SetRoutes.FindNearestFinish(start, finish - i);
      }

    }
  }
  system("pause");
  return 0;
}
*/


int main() {
  RouteManager routes;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    } else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}

