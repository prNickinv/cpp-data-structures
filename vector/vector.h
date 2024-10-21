#ifndef VECTOR_H
#define VECTOR_H
#define VECTOR_MEMORY_IMPLEMENTED

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>

class VectorOutOfRange : public std::out_of_range {
 public:
  VectorOutOfRange() : std::out_of_range("VectorOutOfRange") {
  }
};

template <typename T>
class Vector {
 private:
  T* vector_;
  std::size_t size_;
  std::size_t capacity_;

 public:
  using ValueType = T;
  using SizeType = std::size_t;
  using Reference = T&;
  using ConstReference = const T&;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() : vector_{nullptr}, size_{0}, capacity_{0} {
  }

  explicit Vector(SizeType size) {
    try {
      vector_ = static_cast<Pointer>(operator new(sizeof(ValueType) * size));
      size_ = 0;
      capacity_ = size;
      for (std::size_t i = 0; i != size; ++i) {
        new (vector_ + i) ValueType;
        ++size_;
      }
    } catch (...) {
      std::destroy(vector_, vector_ + size_);
      operator delete(vector_);
      vector_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  Vector(SizeType size, ConstReference value) {
    if (size == 0) {
      vector_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      try {
        vector_ = static_cast<Pointer>(operator new(sizeof(ValueType) * size));
        size_ = 0;
        capacity_ = size;
        for (std::size_t i = 0; i != size; ++i) {
          new (vector_ + i) ValueType(value);
          ++size_;
        }
      } catch (...) {
        std::destroy(vector_, vector_ + size_);
        operator delete(vector_);
        vector_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    if ((std::distance(first, last)) == 0) {
      vector_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      try {
        vector_ = static_cast<Pointer>(operator new(sizeof(ValueType) * std::distance(first, last)));
        size_ = 0;
        capacity_ = std::distance(first, last);
        std::size_t step = 0;
        for (auto it = first; it != last; ++it) {
          new (vector_ + step) ValueType(*it);
          ++step;
          ++size_;
        }
      } catch (...) {
        std::destroy(vector_, vector_ + size_);
        operator delete(vector_);
        vector_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  Vector(std::initializer_list<ValueType> lst) {
    if (lst.size() == 0) {
      vector_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      try {
        vector_ = static_cast<Pointer>(operator new(sizeof(ValueType) * lst.size()));
        size_ = 0;
        capacity_ = lst.size();
        std::size_t step = 0;
        for (auto&& elem : lst) {
          new (vector_ + step) ValueType(elem);
          ++step;
          ++size_;
        }
      } catch (...) {
        std::destroy(vector_, vector_ + size_);
        operator delete(vector_);
        vector_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  ~Vector() {
    std::destroy(vector_, vector_ + size_);
    operator delete(vector_);
    vector_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }

  Vector(const Vector& other) {
    if (other.vector_ == nullptr) {
      vector_ = nullptr;
      size_ = 0;
      capacity_ = 0;
    } else {
      try {
        vector_ = static_cast<Pointer>(operator new(sizeof(ValueType) * other.size_));
        size_ = 0;
        capacity_ = other.size_;
        for (std::size_t i = 0; i != other.size_; ++i) {
          new (vector_ + i) ValueType(other.vector_[i]);
          ++size_;
        }
      } catch (...) {
        std::destroy(vector_, vector_ + size_);
        operator delete(vector_);
        vector_ = nullptr;
        size_ = 0;
        capacity_ = 0;
        throw;
      }
    }
  }

  Vector& operator=(const Vector& other) {
    if (this == &other) {
      return *this;
    }
    if (capacity_ < other.size_) {
      Vector<ValueType> help;
      help.Reserve(other.size_);
      std::uninitialized_copy(other.vector_, other.vector_ + other.size_, help.vector_);
      help.size_ = other.size_;
      Swap(help);
    } else if (size_ >= other.size_) {
      std::copy(other.vector_, other.vector_ + other.size_, vector_);
      std::destroy(vector_ + other.size_, vector_ + size_);
      size_ = other.size_;
    } else {
      std::copy(other.vector_, other.vector_ + size_, vector_);
      std::uninitialized_copy(other.vector_ + size_, other.vector_ + other.size_, vector_ + size_);
      size_ = other.size_;
    }
    return *this;
  }

  Vector(Vector&& other) noexcept {
    vector_ = other.vector_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.vector_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  Vector& operator=(Vector&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    std::destroy(vector_, vector_ + size_);
    operator delete(vector_);
    vector_ = nullptr;

    vector_ = other.vector_;
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.vector_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
  }

  SizeType Size() const {
    return size_;
  }

  SizeType Capacity() const {
    return capacity_;
  }

  bool Empty() const {
    return size_ == 0;
  }

  Reference operator[](SizeType index) {
    return vector_[index];
  }

  ConstReference operator[](SizeType index) const {
    return vector_[index];
  }

  Reference At(SizeType index) {
    if (index >= size_) {
      throw VectorOutOfRange{};
    }
    return vector_[index];
  }

  ConstReference At(SizeType index) const {
    if (index >= size_) {
      throw VectorOutOfRange{};
    }
    return vector_[index];
  }

  Reference Front() {
    return vector_[0];
  }

  ConstReference Front() const {
    return vector_[0];
  }

  Reference Back() {
    return vector_[size_ - 1];
  }

  ConstReference Back() const {
    return vector_[size_ - 1];
  }

  Pointer Data() {
    return vector_;
  }

  ConstPointer Data() const {
    return vector_;
  }

  void Swap(Vector& other) {
    std::swap(vector_, other.vector_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  void Reserve(std::size_t new_capacity) {
    if (new_capacity > capacity_) {
      SizeType size = size_;
      Vector<ValueType> help;
      help.vector_ = static_cast<Pointer>(operator new(sizeof(ValueType) * new_capacity));
      std::uninitialized_move(vector_, vector_ + size_, help.vector_);
      help.size_ = size;
      help.capacity_ = new_capacity;
      help.Swap(*this);
    }
  }

  void Resize(SizeType new_size) {
    if (new_size < size_) {
      std::destroy(vector_ + new_size, vector_ + size_);
      size_ = new_size;
    } else if (new_size > size_) {
      if (new_size > capacity_) {
        Vector<ValueType> temp;
        SizeType size_init = size_;
        temp.Reserve(new_size);
        std::uninitialized_move(vector_, vector_ + size_, temp.vector_);
        temp.size_ = size_init;
        for (std::size_t i = size_init; i != new_size; ++i) {
          ++temp.size_;
          new (temp.vector_ + i) ValueType;
        }
        Swap(temp);
      } else {
        std::uninitialized_default_construct(vector_ + size_, vector_ + new_size);
        size_ = new_size;
      }
    }
  }

  void Resize(SizeType new_size, ConstReference value) {
    if (new_size < size_) {
      std::destroy(vector_ + new_size, vector_ + size_);
      size_ = new_size;
    } else if (new_size > size_) {
      if (new_size > capacity_) {
        Reserve(new_size);
      }
      std::uninitialized_fill(vector_ + size_, vector_ + new_size, value);
      size_ = new_size;
    }
  }

  void ShrinkToFit() {
    if (size_ < capacity_) {
      if (size_ == 0) {
        operator delete(vector_);
        vector_ = nullptr;
        capacity_ = 0;
      } else {
        Vector<ValueType> temp;
        temp.Reserve(size_);
        std::uninitialized_move(vector_, vector_ + size_, temp.vector_);
        temp.size_ = size_;
        Swap(temp);
      }
    }
  }

  void Clear() {
    std::destroy(vector_, vector_ + size_);
    size_ = 0;
  }

  void PushBack(ConstReference elem) {
    if (capacity_ <= size_) {
      SizeType buffer_size = capacity_ == 0 ? 1 : capacity_ * 2;
      Vector<ValueType> temp;
      temp.Reserve(buffer_size);
      std::uninitialized_copy(vector_, vector_ + size_, temp.vector_);
      temp.size_ = size_;
      ++temp.size_;
      std::uninitialized_fill_n(temp.vector_ + size_, 1, elem);
      Swap(temp);
    } else {
      std::uninitialized_fill_n(vector_ + size_, 1, elem);
      ++size_;
    }
  }

  void PushBack(ValueType&& elem) {
    if (capacity_ <= size_) {
      if (capacity_ == 0) {
        Reserve(capacity_ * 2 + 1);
      } else {
        Reserve(capacity_ * 2);
      }
    }
    new (vector_ + size_) ValueType(std::forward<ValueType>(elem));
    ++size_;
  }

  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    if (capacity_ <= size_) {
      if (capacity_ == 0) {
        Reserve(capacity_ * 2 + 1);
      } else {
        Reserve(capacity_ * 2);
      }
    }
    new (vector_ + size_) ValueType(std::forward<Args>(args)...);
    ++size_;
  }

  void PopBack() {
    if (size_ != 0) {
      --size_;
      std::destroy_at(vector_ + size_);
    }
  }

  bool operator==(const Vector& other) const {
    return std::equal(vector_, vector_ + size_, other.vector_, other.vector_ + other.size_);
  }

  bool operator!=(const Vector& other) const {
    return !(*this == other);
  }

  bool operator<(const Vector& other) const {
    return std::lexicographical_compare(vector_, vector_ + size_, other.vector_, other.vector_ + other.size_);
  }

  bool operator<=(const Vector& other) const {
    return *this == other || *this < other;
  }

  bool operator>(const Vector& other) const {
    return !(*this <= other);
  }

  bool operator>=(const Vector& other) const {
    return !(*this < other);
  }

  Iterator begin() {  // NOLINT
    return vector_;
  }

  Iterator end() {  // NOLINT
    return vector_ + size_;
  }

  ConstIterator begin() const {  // NOLINT
    return vector_;
  }

  ConstIterator end() const {  // NOLINT
    return vector_ + size_;
  }

  ConstIterator cbegin() const {  // NOLINT
    return vector_;
  }

  ConstIterator cend() const {  // NOLINT
    return vector_ + size_;
  }

  ReverseIterator rbegin() {  // NOLINT
    return ReverseIterator(end());
  }

  ReverseIterator rend() {  // NOLINT
    return ReverseIterator(begin());
  }

  ConstReverseIterator rbegin() const {  // NOLINT
    return ConstReverseIterator(end());
  }

  ConstReverseIterator rend() const {  // NOLINT
    return ConstReverseIterator(begin());
  }

  ConstReverseIterator crbegin() const {  // NOLINT
    return rbegin();
  }

  ConstReverseIterator crend() const {  // NOLINT
    return rend();
  }
};

#endif