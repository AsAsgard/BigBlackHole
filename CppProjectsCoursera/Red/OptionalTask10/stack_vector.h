#pragma once

#include <stdexcept>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0) : size(0) { if (a_size > N) throw invalid_argument("Bad stack vector size"); size = a_size;}

  T& operator[](size_t index) { return data[index];}
  const T& operator[](size_t index) const { return data[index];}

  T* begin() { return data;}
  T* end() { return data + size;}
  const T* begin() const { return data;}
  const T* end() const { return data + size;}

  size_t Size() const { return size;}
  size_t Capacity() const { return  capacity;}

  void PushBack(const T& value)
  {
      if (size == capacity) throw overflow_error("No more place in stack vector");
      data[size] = value;
      ++size;
  }

  T PopBack()
  {
      if (size == 0) throw underflow_error("Stack vector is already empty");
      --size;
      return data[size];
  }

private:
  T data[N];
  size_t size;
  const size_t capacity = N;
};

