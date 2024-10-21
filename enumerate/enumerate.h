#ifndef ENUMERATE_H
#define ENUMERATE_H
#define TEMPORARY_ENUMERATE_IMPLEMENTED

#include <iterator>

template <class Iterator>
class EnumerateIterator {

 private:
  Iterator iter_;
  std::size_t state_ = 0;

 public:
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;                // NOLINT
  using value_type = std::pair<std::size_t, typename std::iterator_traits<Iterator>::value_type>;  // NOLINT
  using pointer = void;                                                                            // NOLINT
  using reference = std::pair<std::size_t, typename std::iterator_traits<Iterator>::reference>;    // NOLINT
  using iterator_category = std::input_iterator_tag;                                               // NOLINT

  EnumerateIterator() = default;

  EnumerateIterator(Iterator iter, std::size_t state = 0) : iter_(std::move(iter)), state_(state) {  // NOLINT
  }

  reference operator*() {
    return reference(state_, *iter_);
  }

  EnumerateIterator& operator++() {
    ++state_;
    ++iter_;
    return *this;
  }

  EnumerateIterator operator++(int) {
    auto temp = *this;
    ++state_;
    ++iter_;
    return temp;
  }

  Iterator GetIterator() const {
    return iter_;
  }

  ~EnumerateIterator() = default;
};

template <class Iterator>
bool operator==(const EnumerateIterator<Iterator>& lhs, const EnumerateIterator<Iterator>& rhs) {
  return lhs.GetIterator() == rhs.GetIterator();
}

template <class Iterator>
bool operator!=(const EnumerateIterator<Iterator>& lhs, const EnumerateIterator<Iterator>& rhs) {
  return !(lhs == rhs);
}

template <class Iterable>
class EnumerateProxy {
 public:
  using iterator = EnumerateIterator<decltype((std::declval<const Iterable&>()).begin())>;  // NOLINT

  EnumerateProxy(Iterable&& container) : iterable_(std::forward<Iterable>(container)) {  // NOLINT
  }

  iterator begin() const {  // NOLINT
    return iterator(iterable_.begin());
  }

  iterator end() const {  // NOLINT
    return iterator(iterable_.end());
  }

 private:
  const Iterable iterable_;
};

template <class Iterable>
auto Enumerate(Iterable&& container) {
  return EnumerateProxy<Iterable>(std::forward<Iterable>(container));
}

#endif