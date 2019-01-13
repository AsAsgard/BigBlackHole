#include <cstdlib>
#include <algorithm>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

template <typename T>
class SimpleVector {
public:
  SimpleVector()
      : data_(NULL)
      , size_(0)
      , capacity_(0)
  {
  }
  explicit SimpleVector(size_t size)
      : data_(NULL)
      , size_(size)
      , capacity_(size)
  {
      if (size > 0) data_ = new T[size];
  }
  SimpleVector(SimpleVector&& other)
      : data_(other.data_)
      , size_(other.size_)
      , capacity_(other.capacity_)
  {
      other.data_ = NULL;
      other.size_ = 0;
      other.capacity_ = 0;
  }
  ~SimpleVector()
  {
      delete [] data_;
  }

  T& operator[](size_t index) { return data_[index];}
  SimpleVector<T>& operator=(SimpleVector&& other)
  {
      delete [] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;

      other.data_ = NULL;
      other.size_ = 0;
      other.capacity_ = 0;
  }

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
          std::move(data_, data_ + size_, new_data_);
          delete [] data_;
          data_ = new_data_;
      }
      data_[size_] = value;
      ++size_;
  }
  void PushBack(T&& value)
  {
      if (size_ == capacity_) {
          if (size_ == 0)
              ++capacity_;
          else
              capacity_ *= 2;
          T* new_data_ = new T[capacity_];
          std::move(data_, data_ + size_, new_data_);
          delete [] data_;
          data_ = new_data_;
      }
      data_[size_] = move(value);
      ++size_;
  }

  // При необходимости перегрузите
  // существующие публичные методы

private:
  // Добавьте сюда поля
  T* data_;
  size_t size_;
  size_t capacity_;
};
