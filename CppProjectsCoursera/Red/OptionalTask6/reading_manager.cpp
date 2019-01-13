#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <map>

#include "profile.h"
using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : user_page_count(MAX_USER_COUNT_ + 1, 0)
      , users_less_pages(MAX_PAGES_COUNT + 1)
      , user_count(0) {}

  void Read(int user_id, int page_count) {
//    LOG_DURATION("READ");
    if (user_page_count[user_id] == 0) {
        user_page_count[user_id] = page_count;
        ++user_count;
        for (int i = page_count + 1; i <= MAX_PAGES_COUNT; ++i) ++users_less_pages[i];
    } else {
        for (int i = user_page_count[user_id] + 1; i <= page_count; ++i) --users_less_pages[i];
        user_page_count[user_id] = page_count;
    }
  }

  double Cheer(int user_id) const {
//    LOG_DURATION("CHEER");
    if (user_page_count[user_id] == 0) {
      return 0;
    }

    if (user_count == 1) {
      return 1;
    }

    return users_less_pages[user_page_count[user_id]] * 1.0 / (user_count - 1);
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGES_COUNT = 1'000;

  vector<int> user_page_count;
  vector<int> users_less_pages;
  size_t user_count;
};



int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}
