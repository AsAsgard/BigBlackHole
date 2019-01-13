#include "test_runner.h"

#include <vector>
#include <stdexcept>
using namespace std;


template<typename T>
class Deque {
    vector<T> FrontVector;
    vector<T> BackVector;
public:
    Deque(size_t size = 0) { if (size != 0) BackVector.resize(size);}

    size_t Size() const { return FrontVector.size() + BackVector.size();}
    bool Empty() const {return (Size() == 0) ? true : false;}


#define SQ_BRACKETS_IMPL {                                          \
    if (index < FrontVector.size())                                 \
        return FrontVector[FrontVector.size() - 1 - index];         \
    else                                                            \
        return BackVector[index - FrontVector.size()];              \
}

    T& operator[](size_t index) { SQ_BRACKETS_IMPL;}
    const T& operator[](size_t index) const { SQ_BRACKETS_IMPL;}


#define AT_IMPL {                           \
    if (index >= Size() || index < 0)       \
        throw out_of_range("Bad index");    \
    else                                    \
        return (*this)[index];              \
}

    T& At(size_t index) { AT_IMPL;}
    const T& At(size_t index) const { AT_IMPL;}

    void PushBack(const T& value) { BackVector.push_back(value);}
    void PushFront(const T& value) { FrontVector.push_back(value);}


#define GET_LAST(place) return place##Vector.back()

#define GET_FIRST(place) return place##Vector.front()

#define GET(place1, place2) {                       \
    if (Empty()) throw out_of_range("No elements"); \
    if (!place1##Vector.empty())                    \
        GET_LAST(place1);                           \
    else                                            \
        GET_FIRST(place2);                           \
}

    T& Front() { GET(Front, Back);}
    const T& Front() const { GET(Front, Back);}
    T& Back() { GET(Back, Front);}
    const T& Back() const { GET(Back, Front);}
};

void TestEmpty() {
  Deque<int> dq;
  ASSERT(dq.Empty());
}

void TestPush() {
  Deque<int> dq;
  dq.PushBack(2);
  dq.PushFront(1);
  ASSERT(!dq.Empty());
}

void TestSize() {
  Deque<int> dq;
  dq.PushBack(2);
  dq.PushFront(1);
  ASSERT_EQUAL(dq.Size(), 2u);
}

void TestFront() {
  Deque<int> dq;
  dq.PushFront(1);
  ASSERT_EQUAL(dq.Front(), 1);

  dq.Front() = 0;
  ASSERT_EQUAL(dq.Front(), 0);
}

void TestBack() {
  Deque<int> dq;
  dq.PushBack(1);
  ASSERT_EQUAL(dq.Back(), 1);

  dq.Back() = 0;
  ASSERT_EQUAL(dq.Back(), 0);
}

void TestOperator() {
  // front и back имеют элементы
  Deque<int> dq_of_int;
  dq_of_int.PushFront(1);
  dq_of_int.PushFront(2);
  dq_of_int.PushFront(3);
  dq_of_int.PushBack(4);
  dq_of_int.PushBack(5);
  ASSERT_EQUAL(dq_of_int[0], 3);
  ASSERT_EQUAL(dq_of_int[1], 2);
  ASSERT_EQUAL(dq_of_int[2], 1);
  ASSERT_EQUAL(dq_of_int[3], 4);
  ASSERT_EQUAL(dq_of_int[4], 5);

  // front имеет элементы, back – пуст
  Deque<double> dq_of_db;
  dq_of_db.PushFront(1.5);
  dq_of_db.PushFront(2.5);
  dq_of_db.PushFront(3.5);
  ASSERT_EQUAL(dq_of_db[0], 3.5);
  ASSERT_EQUAL(dq_of_db[1], 2.5);
  ASSERT_EQUAL(dq_of_db[2], 1.5);

  // back имеет элементы, front – пуст
  Deque<char> dq_of_ch;
  dq_of_ch.PushBack('a');
  dq_of_ch.PushBack('b');
  ASSERT_EQUAL(dq_of_ch[0], 'a');
}

void TestAt() {
  // front и back имеют элементы
  Deque<int> dq_of_int;
  dq_of_int.PushFront(1);
  dq_of_int.PushFront(2);
  dq_of_int.PushFront(3);
  dq_of_int.PushBack(4);
  dq_of_int.PushBack(5);
  ASSERT_EQUAL(dq_of_int.At(0), 3);
  ASSERT_EQUAL(dq_of_int.At(1), 2);
  ASSERT_EQUAL(dq_of_int.At(2), 1);
  ASSERT_EQUAL(dq_of_int.At(3), 4);
  ASSERT_EQUAL(dq_of_int.At(4), 5);

  // front имеет элементы, back – пуст
  Deque<double> dq_of_db;
  dq_of_db.PushFront(1.5);
  dq_of_db.PushFront(2.5);
  dq_of_db.PushFront(3.5);
  ASSERT_EQUAL(dq_of_db.At(0), 3.5);
  ASSERT_EQUAL(dq_of_db.At(1), 2.5);
  ASSERT_EQUAL(dq_of_db.At(2), 1.5);

  try {
    dq_of_db.At(3);
  } catch (const out_of_range& ex) {
    bool exception_caught = true;
    ASSERT(exception_caught);
  }

  // back имеет элементы, front – пуст
  Deque<char> dq_of_ch;
  dq_of_ch.PushBack('a');
  dq_of_ch.PushBack('b');
  ASSERT_EQUAL(dq_of_ch.At(0), 'a');
}

void FinalTest() {
    Deque<string> dq;
    dq.PushBack("A");
    ASSERT_EQUAL(dq.Front(), "A");
    ASSERT_EQUAL(dq.Back(), "A");
    dq.PushBack("B");
    dq.PushBack("C");
    dq.PushBack("D");
    ASSERT_EQUAL(dq.Front(), "A");
    ASSERT_EQUAL(dq.Back(), "D");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEmpty);
  RUN_TEST(tr, TestPush);
  RUN_TEST(tr, TestSize);
  RUN_TEST(tr, TestFront);
  RUN_TEST(tr, TestBack);
  RUN_TEST(tr, TestOperator);
  RUN_TEST(tr, TestAt);
  RUN_TEST(tr, FinalTest);
  return 0;
}
