#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <queue>

using namespace std;

template <typename T>
class PriorityCollection {
    struct Node {
        Node(int _priority, T&& _object, int _order) : priority(_priority), object(move(_object)), order(_order) {}

        int priority;
        T object;
        int order;
    };

    struct PriorWithOrder {
        PriorWithOrder(int _priority, int _order) : priority(_priority), order(_order) {}

        int priority;
        int order;

        friend bool operator>(const PriorWithOrder& lhs, const PriorWithOrder& rhs)
        {
            return tie(lhs.priority, lhs.order) > tie(rhs.priority, rhs.order);
        }
    };

public:
    using Id = unsigned long long;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор O(1)
    Id Add(T object) {
        Id id = AddElement(move(object));

        top_priority.first = ptrs[priority_to_Ids.begin()->second];
        top_priority.second = priority_to_Ids.begin()->second;

        return id;
    }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...) O(N*logN)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin)
    {
        Id id = 0;
        vector<Id> ids;

        while (range_begin != range_end)
        {
            id = AddElement(move(*range_begin++));
            ids.push_back(id);
        }

        move(ids.begin(), ids.end(), ids_begin);

        top_priority.first = ptrs[priority_to_Ids.begin()->second];
        top_priority.second = priority_to_Ids.begin()->second;
    }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту O(1)
    bool IsValid(Id id) const
    {
        if (ptrs.size() > id)
            if (ptrs[id] != data_.end()) return true;
        return false;
    }

  // Получить объект по идентификатору O(1)
    const T& Get(Id id) const
    {
        return ptrs[id]->object;
    }

  // Увеличить приоритет объекта на 1 O(logN)
    void Promote(Id id)
    {
        priority_to_Ids.erase({ptrs[id]->priority, ptrs[id]->order});
        ++ptrs[id]->priority;
        priority_to_Ids.emplace(PriorWithOrder{ptrs[id]->priority, ptrs[id]->order}, id);
        top_priority.first = ptrs[priority_to_Ids.begin()->second];
        top_priority.second = priority_to_Ids.begin()->second;
    }

  // Получить объект с максимальным приоритетом и его приоритет O(1)
    pair<const T&, int> GetMax() const
    {
        return {top_priority.first->object, top_priority.first->priority};
    }

  // Аналогично GetMax, но удаляет элемент из контейнера O(logN)
    pair<T, int> PopMax()
    {
        ptrs[top_priority.second] = data_.end();
        free_id.push(top_priority.second);
        auto result = make_pair(move(top_priority.first->object), top_priority.first->priority);
        priority_to_Ids.erase({top_priority.first->priority, top_priority.first->order});
        data_.erase(top_priority.first);

        if (!priority_to_Ids.empty()) {
            top_priority.first = ptrs[priority_to_Ids.begin()->second];
            top_priority.second = priority_to_Ids.begin()->second;
        } else {
            top_priority.first = data_.end();
            top_priority.second = 0;
        }

        return result;
    }

private:
  // Приватные поля и методы
    list<Node> data_;
    vector<typename list<Node>::iterator> ptrs;
    queue<Id> free_id;
    map<PriorWithOrder, Id, greater<PriorWithOrder>> priority_to_Ids;

    pair<typename list<Node>::iterator, Id> top_priority = {data_.end(), 0};
    Id last_id = 0;
    int current_order = 0;

    // добавление одного элемента O(logN)
    Id AddElement(T&& object)
    {
        data_.emplace_back(0, move(object), current_order);
        auto inserted_elem_it = prev(data_.end());

        Id id = 0;
        if (free_id.empty())  {
            ptrs.push_back(inserted_elem_it);
            id = last_id++;
        } else {
            id = free_id.front();
            ptrs[id] = inserted_elem_it;
            free_id.pop();
        }

        priority_to_Ids.emplace(PriorWithOrder{0, current_order}, id);

        ++current_order;

        return id;
    }
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
