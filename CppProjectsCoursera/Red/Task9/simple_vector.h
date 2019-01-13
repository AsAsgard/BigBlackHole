#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector() : data_(NULL), size_(0), capacity_(0) {}
  explicit SimpleVector(size_t size) : data_(new T[size]), size_(size), capacity_(size) {}
  ~SimpleVector() { delete [] data_;}

  T& operator[](size_t index) { return data_[index];}

  T* begin() { return data_;}
  T* end() { return data_ + size_;}

  const T* begin() const { return data_;}
  const T* end() const { return data_ + size_;}

  size_t Size() const { return size_;}
  size_t Capacity() const { return capacity_;}
  void PushBack(const T& value)
  {
      if (size_ == capacity_) {
          if (size_ == 0)
              ++capacity_;
          else
              capacity_ *= 2;
          T* new_data_ = new T[capacity_];
          std::copy(data_, data_ + size_, new_data_);
          delete [] data_;
          data_ = new_data_;
      }
      data_[size_] = value;
      ++size_;
  }

private:
  // Добавьте поля для хранения данных вектора
  T* data_;
  std::size_t size_;
  std::size_t capacity_;
};
