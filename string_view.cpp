#include "string_view.h"

StringView::StringView() : string_{nullptr}, size_{0} {
}

StringView::StringView(const char* string) : string_{string}, size_{strlen(string)} {
}

StringView::StringView(const char* string, std::size_t size) : string_{string}, size_{size} {
}

const char& StringView::operator[](std::size_t index) const {
  return string_[index];
}

const char& StringView::At(std::size_t index) const {
  if (index >= size_) {
    throw StringViewOutOfRange{};
  }
  return string_[index];
}

const char& StringView::Front() const {
  return string_[0];
}

const char& StringView::Back() const {
  return string_[size_ - 1];
}

std::size_t StringView::Size() const {
  return size_;
}

std::size_t StringView::Length() const {
  return size_;
}

bool StringView::Empty() const {
  return size_ == 0;
}

const char* StringView::Data() const {
  return string_;
}

void StringView::Swap(StringView& str) {
  const char* placeholder = string_;
  string_ = str.string_;
  str.string_ = placeholder;
  std::size_t size_ph = size_;
  size_ = str.size_;
  str.size_ = size_ph;
}

void StringView::RemovePrefix(std::size_t prefix_size) {
  string_ += prefix_size;
  size_ -= prefix_size;
}

void StringView::RemoveSuffix(std::size_t suffix_size) {
  size_ -= suffix_size;
}

StringView StringView::Substr(std::size_t pos, std::size_t count = -1) const {
  if (pos > size_) {
    throw StringViewOutOfRange{};
  }
  std::size_t substr_len = (count <= (size_ - pos)) ? count : (size_ - pos);
  return {string_ + pos, substr_len};
}
