#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

template <typename T>
class UniquePtr {
 private:
  T* data_ = nullptr;

 public:
  UniquePtr() : data_{nullptr} {};
  explicit UniquePtr(T* ptr) : data_{ptr} {};

  explicit UniquePtr(const UniquePtr& source) = delete;
  UniquePtr& operator=(const UniquePtr& source) = delete;

  ~UniquePtr() {
    delete data_;
  }

  UniquePtr(UniquePtr&& source) noexcept {
    data_ = source.data_;
    source.data_ = nullptr;
  }

  UniquePtr& operator=(UniquePtr&& source) noexcept {
    if (this == &source) {
      return *this;
    }
    delete data_;
    data_ = source.data_;
    source.data_ = nullptr;
    return *this;
  }

  T* Release() {
    T* temp = data_;
    data_ = nullptr;
    return temp;
  }

  void Reset(T* ptr = nullptr) {
    delete data_;
    data_ = ptr;
  }

  void Swap(UniquePtr<T>& other) {
    T* temp = data_;
    data_ = other.data_;
    other.data_ = temp;
  }

  T* Get() const {
    return data_;
  }

  T& operator*() const {
    return *data_;
  }

  T* operator->() const {
    return data_;
  }

  explicit operator bool() const {
    return static_cast<bool>(data_);
  }
};

#endif
