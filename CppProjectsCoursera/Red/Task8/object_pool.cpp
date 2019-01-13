#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate()
  {
    if (qu_ptr.empty()) {
      T* current_ptr = new T;
      set_ptr.insert(current_ptr);
      return current_ptr;
    } else {
      T* current_ptr = qu_ptr.front();
      qu_ptr.pop();
      set_ptr.insert(current_ptr);
      return current_ptr;
    }
  }

  T* TryAllocate()
  {
    if (qu_ptr.empty()) {
        return nullptr;
    } else {
        T* current_ptr = qu_ptr.front();
        qu_ptr.pop();
        set_ptr.insert(current_ptr);
        return current_ptr;
    }
  }


  void Deallocate(T* object)
  {
    if (set_ptr.count(object) < 1) {
      throw invalid_argument("This pointer was not allocated");
    } else {
      set_ptr.erase(object);
      qu_ptr.push(object);
    }
  }

  ~ObjectPool()
  {
    while (!qu_ptr.empty())
    {
        T* current_ptr = qu_ptr.front();
        qu_ptr.pop();
        delete current_ptr;
    }
    for (auto it = set_ptr.begin(); it != set_ptr.end(); ++it)
    {
        delete *it;
    }
    set_ptr.clear();
  }

private:
  queue<T*> qu_ptr;
  set<T*> set_ptr;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
