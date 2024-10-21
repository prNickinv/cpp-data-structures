#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#define WEAK_PTR_IMPLEMENTED

#include <stdexcept>

class BadWeakPtr : public std::runtime_error {
 public:
  BadWeakPtr() : std::runtime_error("BadWeakPtr") {
  }
};

template <typename T>
class WeakPtr;

struct Counter {
  int strong_count_;
  int weak_count_;

  Counter() : strong_count_{0}, weak_count_{0} {
  }

  ~Counter() = default;
};

template <typename T>
class SharedPtr {
 private:
  T* data_;
  Counter* counter_;

 public:
  SharedPtr() : data_{nullptr}, counter_{nullptr} {
  }

  SharedPtr(T* ptr) {  // NOLINT
    data_ = ptr;
    counter_ = nullptr;
    if (ptr) {
      counter_ = new Counter();
      ++counter_->strong_count_;
    }
  }

  SharedPtr(const SharedPtr& other) {
    data_ = other.data_;
    counter_ = other.counter_;
    if (data_) {
      ++counter_->strong_count_;
    }
  }

  SharedPtr& operator=(const SharedPtr& other) {
    if (this == &other) {
      return *this;
    }

    if (counter_) {
      --counter_->strong_count_;
      if (counter_->strong_count_ == 0) {
        delete data_;
        if (counter_->weak_count_ == 0) {
          delete counter_;
        }
      }
    }

    data_ = other.data_;
    counter_ = other.counter_;
    if (data_) {
      ++counter_->strong_count_;
    }
    return *this;
  }

  SharedPtr(SharedPtr&& other) noexcept {
    data_ = other.data_;
    other.data_ = nullptr;

    counter_ = other.counter_;
    other.counter_ = nullptr;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    if (counter_) {
      --counter_->strong_count_;
      if (counter_->strong_count_ == 0) {
        delete data_;
        if (counter_->weak_count_ == 0) {
          delete counter_;
        }
      }
    }
    data_ = other.data_;
    counter_ = other.counter_;
    other.data_ = nullptr;
    other.counter_ = nullptr;
    return *this;
  }

  SharedPtr(const WeakPtr<T>& weak) {  // NOLINT
    if (weak.Expired()) {
      throw BadWeakPtr{};
    }
    data_ = weak.GetWeak();
    counter_ = weak.GetCounterWeak();
    ++counter_->strong_count_;
  }

  ~SharedPtr() {
    if (counter_) {
      --counter_->strong_count_;
      if (counter_->strong_count_ == 0) {
        delete data_;
        data_ = nullptr;
        if (counter_->weak_count_ == 0) {
          delete counter_;
          counter_ = nullptr;
        }
      }
    }
  }

  void Reset(T* ptr = nullptr) {
    if (counter_) {
      --counter_->strong_count_;
      if (counter_->strong_count_ == 0) {
        delete data_;
        if (counter_->weak_count_ == 0) {
          delete counter_;
        }
      }
    }
    data_ = ptr;
    counter_ = nullptr;
    if (data_) {
      counter_ = new Counter();
      ++counter_->strong_count_;
    }
  }

  void Swap(SharedPtr<T>& other) {
    std::swap(data_, other.data_);
    std::swap(counter_, other.counter_);
  }

  T* Get() const {
    return data_;
  }

  int UseCount() const {
    if (counter_) {
      return counter_->strong_count_;
    }
    return 0;
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

  Counter* GetCounter() const {
    return counter_;
  }

  // friend class WeakPtr<T>;
};

template <typename T>
class WeakPtr {
 private:
  T* data_;
  Counter* counter_;

 public:
  WeakPtr() : data_{nullptr}, counter_{nullptr} {
  }

  WeakPtr(const WeakPtr& other) {
    data_ = other.data_;
    counter_ = other.counter_;
    if (data_) {
      ++counter_->weak_count_;
    }
  }

  WeakPtr& operator=(WeakPtr& other) {
    if (this == &other) {
      return *this;
    }

    if (counter_) {
      --counter_->weak_count_;
      if (counter_->weak_count_ == 0 && counter_->strong_count_ == 0) {
        delete counter_;
        counter_ = nullptr;
      }
    }

    data_ = other.data_;
    counter_ = other.counter_;
    if (data_) {
      ++counter_->weak_count_;
    }
    return *this;
  }

  WeakPtr(WeakPtr&& other) noexcept {
    data_ = other.data_;
    counter_ = other.counter_;

    other.data_ = nullptr;
    other.counter_ = nullptr;
  }

  WeakPtr& operator=(WeakPtr&& other) noexcept {
    if (this == &other) {
      return *this;
    }

    if (counter_) {
      --counter_->weak_count_;
      if (counter_->weak_count_ == 0 && counter_->strong_count_ == 0) {
        delete counter_;
        counter_ = nullptr;
      }
    }

    data_ = other.data_;
    counter_ = other.counter_;
    other.data_ = nullptr;
    other.counter_ = nullptr;
    return *this;
  }

  WeakPtr(const SharedPtr<T>& shared) {  // NOLINT
    data_ = shared.Get();
    counter_ = shared.GetCounter();
    if (data_) {
      ++counter_->weak_count_;
    }
  }

  ~WeakPtr() {
    if (counter_) {
      --counter_->weak_count_;
      if (counter_->weak_count_ == 0 && counter_->strong_count_ == 0) {
        delete counter_;
        counter_ = nullptr;
      }
    }
  }

  void Swap(WeakPtr<T>& other) {
    std::swap(data_, other.data_);
    std::swap(counter_, other.counter_);
  }

  void Reset() {
    if (counter_) {
      --counter_->weak_count_;
      if (counter_->weak_count_ == 0 && counter_->strong_count_ == 0) {
        delete counter_;
        counter_ = nullptr;
      }
    }
    data_ = nullptr;
    counter_ = nullptr;
  }

  int UseCount() const {
    if (counter_) {
      return counter_->strong_count_;
    }
    return 0;
  }

  bool Expired() const {
    return counter_ == nullptr || counter_->strong_count_ == 0;
  }

  SharedPtr<T> Lock() const {
    if (Expired()) {
      return SharedPtr<T>();
    }
    return SharedPtr<T>(*this);
  }

  T* GetWeak() const {
    return data_;
  }

  Counter* GetCounterWeak() const {
    return counter_;
  }
};

template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args) {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

#endif