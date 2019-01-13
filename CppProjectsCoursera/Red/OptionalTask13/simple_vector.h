#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector() : data_(NULL), size_(0), capacity_(0) {}
  explicit SimpleVector(size_t size) : data_(NULL), size_(size), capacity_(size) { if (size > 0) data_ = new T[size];}
  SimpleVector(const SimpleVector& other) : data_(NULL), size_(other.size_), capacity_(other.capacity_)
  {
      if (other.data_) {
        data_ = new T[other.capacity_];
        std::copy(other.begin(), other.end(), begin());
      }
  }
  ~SimpleVector() { delete [] data_;}

  SimpleVector<T>& operator=(const SimpleVector& other)
  {
      if (other.size_ <= capacity_) {
          if (other.size_ > 0) {
              std::copy(other.begin(), other.end(), begin());
              size_ = other.size_;
          } else {
              delete [] data_;
              data_ = NULL;
              size_ = 0;
              capacity_ = 0;
          }
      } else {
          SimpleVector<T> tmp(other);
          std::swap(tmp.data_, data_);
          std::swap(tmp.size_, size_);
          std::swap(tmp.capacity_, capacity_);
      }
      return *this;
  }
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
