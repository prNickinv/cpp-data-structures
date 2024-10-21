#ifndef REVERSED_H
#define REVERSED_H
#define REVERSE_REVERSED_IMPLEMENTED
#define TEMPORARY_REVERSED_IMPLEMENTED

#include <iterator>

template <class Iterator>
class ReversedIterator {
 private:
  Iterator iter_;

 public:
  using difference_type = typename std::iterator_traits<Iterator>::difference_type;  // NOLINT
  using value_type = typename std::iterator_traits<Iterator>::value_type;            // NOLINT
  using pointer = void;                                                              // NOLINT
  using reference = typename std::iterator_traits<Iterator>::reference;              // NOLINT
  using iterator_category = std::bidirectional_iterator_tag;                         // NOLINT

  ReversedIterator() = default;

  ReversedIterator(Iterator iter) : iter_{iter} {  // NOLINT
  }

  ~ReversedIterator() = default;

  reference operator*() {
    Iterator temp = iter_;
    return *--temp;
  }

  ReversedIterator& operator--() {
    ++iter_;
    return *this;
  }

  ReversedIterator operator--(int) {
    auto temp = *this;
    ++iter_;
    return temp;
  }

  ReversedIterator& operator++() {
    --iter_;
    return *this;
  }

  ReversedIterator operator++(int) {
    auto temp = *this;
    --iter_;
    return temp;
  }

  Iterator GetIterator() const {
    return iter_;
  }
};

template <class Iterator>
bool operator==(const ReversedIterator<Iterator>& lhs, const ReversedIterator<Iterator> rhs) {
  return lhs.GetIterator() == rhs.GetIterator();
}

template <class Iterator>
bool operator!=(const ReversedIterator<Iterator>& lhs, const ReversedIterator<Iterator> rhs) {
  return !(lhs == rhs);
}

template <class Iterable>
class ReversedProxy {
 public:
  using iterator = ReversedIterator<decltype((std::declval<const Iterable&>()).begin())>;  // NOLINT
  using reverse_itetator = std::reverse_iterator<iterator>;                                // NOLINT

  ReversedProxy(Iterable&& container) : iterable_{std::forward<Iterable>(container)} {  // NOLINT
  }

  iterator begin() const {  // NOLINT
    return iterator(std::end(iterable_));
  }

  iterator end() const {  // NOLINT
    return iterator(std::begin(iterable_));
  }

  reverse_itetator rbegin() const {  // NOLINT
    return std::make_reverse_iterator(end());
  }

  reverse_itetator rend() const {  // NOLINT
    return std::make_reverse_iterator(begin());
  }

 private:
  const Iterable iterable_;
};

template <class Iterable>
auto Reversed(Iterable&& container) {
  return ReversedProxy<Iterable>(std::forward<Iterable>(container));
}

#endif