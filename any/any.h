#ifndef ANY_H
#define ANY_H

#include <memory>
#include <stdexcept>

class BadAnyCast : public std::bad_cast {
 public:
  [[nodiscard]] const char* what() const noexcept override {
    return "BadAnyCast error";
  }
};

class Any {
 private:
  template <typename T>
  friend T AnyCast(const Any& value);

  class IHolder {
   public:
    virtual ~IHolder() = default;
    virtual std::shared_ptr<IHolder> Clone() = 0;
  };

 public:
  Any() : pointer_{nullptr} {
  }

  template <typename T>
  Any(T&& other) {  // NOLINT
    pointer_ = std::make_shared<AnyHolder<T>>(std::forward<T>(other));
  }

  template <typename T>
  Any& operator=(T&& other) {
    Any(std::forward<T>(other)).Swap(*this);
    return *this;
  }

  ~Any() = default;

  Any(const Any& other) {
    pointer_ = other.pointer_->Clone();
  }

  Any(Any&& other) noexcept {
    pointer_ = other.pointer_->Clone();
    other.pointer_ = nullptr;
  }

  Any& operator=(const Any& other) {
    Any(other).Swap(*this);
    return *this;
  }

  Any& operator=(Any& other) {
    *this = static_cast<const Any&>(other);
    return *this;
  }

  Any& operator=(Any&& other) noexcept {
    Any(std::move(other)).Swap(*this);
    return *this;
  }

  void Swap(Any& other) {
    std::swap(pointer_, other.pointer_);
  }

  void Reset() noexcept {
    pointer_.reset();
  }

  bool HasValue() const noexcept {
    return static_cast<bool>(pointer_);
  }

  template <typename T>
  class AnyHolder : public IHolder {
   public:
    T data_;

    AnyHolder(T&& other) : data_{std::forward<T>(other)} {  // NOLINT
    }

    std::shared_ptr<IHolder> Clone() override {
      return std::make_shared<AnyHolder<T>>(std::forward<T>(data_));
    }
  };

 private:
  std::shared_ptr<IHolder> pointer_;
};

template <typename T>
T AnyCast(const Any& value) {
  auto temp = dynamic_cast<Any::AnyHolder<T>*>(value.pointer_.get());
  if (temp) {
    return static_cast<T>(temp->data_);
  }
  throw BadAnyCast{};
}

#endif