#ifndef RANGE_H
#define RANGE_H
#define REVERSE_RANGE_IMPLEMENTED

#include <iterator>

class RangeIterator {
 private:
  int64_t state_;
  int64_t step_ = 1;

 public:
  using difference_type = int64_t;                            // NOLINT
  using value_type = int64_t;                                 // NOLINT
  using pointer = void;                                       // NOLINT
  using reference = int64_t;                                  // NOLINT
  using iterator_category = std::bidirectional_iterator_tag;  // NOLINT

  RangeIterator() noexcept : state_{0} {
  }

  explicit RangeIterator(value_type start) noexcept : state_{start} {
  }

  RangeIterator(value_type start, value_type step) noexcept : state_{start}, step_{step} {
  }

  RangeIterator operator++(int) {
    auto temp = *this;
    state_ += step_;
    return temp;
  }

  RangeIterator& operator++() {
    state_ += step_;
    return *this;
  }

  RangeIterator operator--(int) {
    auto temp = *this;
    state_ -= step_;
    return temp;
  }

  RangeIterator& operator--() {
    state_ -= step_;
    return *this;
  }

  reference operator*() const noexcept {
    return state_;
  }
};

inline bool operator==(const RangeIterator& lhs, const RangeIterator& rhs) {
  return *lhs == *rhs;
}

inline bool operator!=(const RangeIterator& lhs, const RangeIterator& rhs) {
  return !(lhs == rhs);
}

class RangeProxy {
 public:
  using value_type = RangeIterator::value_type;              // NOLINT
  using iterator = RangeIterator;                            // NOLINT
  using reverse_itetator = std::reverse_iterator<iterator>;  // NOLINT

  explicit RangeProxy(value_type end) noexcept {
    if (end < 0) {
      end_ = 0;
    } else {
      end_ = end;
    }
  }

  RangeProxy(value_type begin, value_type end, value_type step = 1) noexcept : begin_{begin}, end_{end}, step_{step} {
    if ((end < begin && step > 0) || (begin < end && step < 0) || step == 0) {
      end_ = begin;
      return;
    }

    const auto jump = (end_ - begin_) % step_;
    if (jump != 0) {
      end_ += step_ - jump;
    }
  }

  iterator begin() const noexcept {  // NOLINT
    return {begin_, step_};
  }

  iterator end() const noexcept {  // NOLINT
    return {end_, step_};
  }

  reverse_itetator rbegin() const noexcept {  // NOLINT
    return std::make_reverse_iterator(end());
  }

  reverse_itetator rend() const noexcept {  // NOLINT
    return std::make_reverse_iterator(begin());
  }

 private:
  value_type begin_ = 0;
  value_type end_ = 0;
  value_type step_ = 1;
};

inline auto Range(int64_t end) {
  return RangeProxy(end);
}

inline auto Range(int64_t begin, int64_t end, int64_t step = 1) {
  return RangeProxy(begin, end, step);
}

#endif