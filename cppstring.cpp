#include "cppstring.h"

String::String() : string_{nullptr}, size_{0}, capacity_{0} {
}

String::String(std::size_t size, char symbol) {
  if (size == 0) {
    string_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  } else {
    string_ = new char[size + 1];
    size_ = size;
    capacity_ = size;
    for (std::size_t i = 0; i != size; ++i) {
      string_[i] = symbol;
    }
    string_[size] = '\0';
  }
}

String::String(const char* cstyle) {
  std::size_t size = Strlen(cstyle);
  string_ = new char[size + 1];
  size_ = size;
  capacity_ = size;
  for (std::size_t i = 0; i != size; ++i) {
    string_[i] = cstyle[i];
  }
  string_[size] = '\0';
}

String::String(const char* cstyle, std::size_t size) : string_{new char[size + 1]}, size_{size}, capacity_{size} {
  for (std::size_t i = 0; i != size; ++i) {
    string_[i] = cstyle[i];
  }
  string_[size] = '\0';
}

String::String(const String& copy) {
  if (copy.string_ == nullptr) {
    string_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  } else {
    std::size_t size = copy.size_;
    string_ = new char[size + 1];
    size_ = size;
    capacity_ = size;
    for (std::size_t i = 0; i != size; ++i) {
      string_[i] = copy.string_[i];
    }
    string_[size] = '\0';
  }
}

String& String::operator=(const String& copy) {
  if (this == &copy) {
    return *this;
  }
  delete[] string_;
  string_ = nullptr;
  size_ = copy.size_;
  capacity_ = copy.capacity_;
  if (size_) {
    string_ = new char[size_ + 1];
  }
  for (std::size_t i = 0; i != size_; ++i) {
    string_[i] = copy.string_[i];
  }
  return *this;
}

String::~String() {
  delete[] string_;
  string_ = nullptr;
  size_ = 0;
  capacity_ = 0;
}

const char& String::operator[](std::size_t index) const {
  return string_[index];
}

char& String::operator[](std::size_t index) {
  return string_[index];
}

const char& String::At(std::size_t index) const {
  if (index >= size_) {
    throw StringOutOfRange{};
  }
  return string_[index];
}

char& String::At(std::size_t index) {
  if (index >= size_) {
    throw StringOutOfRange{};
  }
  return string_[index];
}

const char& String::Front() const {
  return string_[0];
}

char& String::Front() {
  return string_[0];
}

const char& String::Back() const {
  return string_[size_ - 1];
}

char& String::Back() {
  return string_[size_ - 1];
}

const char* String::CStr() const {
  return string_;
}

char* String::CStr() {
  return string_;
}

const char* String::Data() const {
  return string_;
}

bool String::Empty() const {
  return size_ == 0;
}

std::size_t String::Size() const {
  return size_;
}

std::size_t String::Length() const {
  return size_;
}

std::size_t String::Capacity() const {
  return capacity_;
}

void String::Clear() {
  if (string_ != nullptr) {
    string_[0] = '\0';
    size_ = 0;
  }
}

void String::Swap(String& other) {
  String temp = *this;
  *this = other;
  other = temp;
}

void String::PopBack() {
  if (string_ != nullptr && size_ != 0) {
    string_[size_ - 1] = '\0';
    --size_;
  }
}

void String::PushBack(char symbol) {
  if (capacity_ <= size_) {
    if (capacity_ == 0) {
      Reserve(capacity_ * 2 + 1);
    } else {
      Reserve(capacity_ * 2);
    }
  }
  string_[size_] = symbol;
  ++size_;
  string_[size_] = '\0';
}

String& String::operator+=(const String& other) {
  std::size_t size = other.size_;
  if (size_ + size >= capacity_) {
    if (size_ + size < capacity_ * 2) {
      if (capacity_ == 0) {
        Reserve(capacity_ * 2 + 1);
      } else {
        Reserve(capacity_ * 2);
      }
    } else {
      Reserve(size_ + size);
    }
  }
  for (std::size_t i = 0; i != size; ++i) {
    string_[i + size_] = other.string_[i];
  }
  size_ += size;
  string_[size_] = '\0';
  return *this;
}

void String::Resize(std::size_t new_size, char symbol) {
  if (new_size == 0 && size_ == 0) {
    return;
  }
  if (new_size <= size_) {
    string_[new_size] = '\0';
    size_ = new_size;
  } else {
    if (new_size >= capacity_) {
      if (new_size < capacity_ * 2) {
        Reserve(capacity_ * 2);
      } else {
        Reserve(new_size);
      }
    }
    for (std::size_t i = size_; i != new_size; ++i) {
      string_[i] = symbol;
    }
    size_ = new_size;
    string_[size_] = '\0';
  }
}

void String::Reserve(std::size_t new_capacity) {
  if (new_capacity > capacity_) {
    auto temp = new char[new_capacity + 1];
    for (std::size_t i = 0; i != size_; ++i) {
      temp[i] = string_[i];
    }
    temp[size_] = '\0';
    delete[] string_;
    string_ = temp;
    capacity_ = new_capacity;
  }
}

void String::ShrinkToFit() {
  if (string_ == nullptr) {
    capacity_ = 0;
  }
  if (size_ < capacity_) {
    auto temp = new char[size_ + 1];
    for (std::size_t i = 0; i != size_; ++i) {
      temp[i] = string_[i];
    }
    temp[size_] = '\0';
    delete[] string_;
    string_ = temp;
    capacity_ = size_;
  }
}

String String::operator+(const String& other) const {
  auto temp = new char[size_ + other.size_ + 1];
  for (std::size_t i = 0; i != size_; ++i) {
    temp[i] = string_[i];
  }
  for (std::size_t i = 0; i != other.size_; ++i) {
    temp[i + size_] = other.string_[i];
  }
  temp[size_ + other.size_] = '\0';
  String res{temp};
  delete[] temp;
  return res;
}

String String::operator+(const char* str) const {
  return *this + String(str);
}

String operator+(const char* str, const String& string) {
  return String(str) + string;
}

bool String::operator==(const String& other) const {
  return Strcmp(string_, other.string_) == 0;
}

bool String::operator!=(const String& other) const {
  return !(*this == other);
}

bool String::operator<(const String& other) const {
  return Strcmp(string_, other.string_) < 0;
}

bool String::operator<=(const String& other) const {
  return *this == other || *this < other;
}

bool String::operator>(const String& other) const {
  return !(*this <= other);
}

bool String::operator>=(const String& other) const {
  return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, const String& str) {
  for (std::size_t i = 0; i != str.size_; ++i) {
    os << str.string_[i];
  }
  return os;
}

std::istream& operator>>(std::istream& is, String& str) {
  str.Clear();
  while (true) {
    if (is.eof()) {
      break;
    }
    char c;
    is >> c;
    if (c == ' ') {
      break;
    }
    str.PushBack(c);
  }
  return is;
}

std::size_t String::Strlen(const char* str) {
  std::size_t size = 0;
  if (str == nullptr) {
    return size;
  }
  while (str[size] != '\0') {
    ++size;
  }
  return size;
}

int String::Strcmp(const char* first, const char* second) {
  if (first == nullptr && second == nullptr) {
    return 0;
  }
  if (first == nullptr) {
    return -1;
  }
  if (second == nullptr) {
    return 1;
  }
  std::size_t i{0};
  while (first[i] == second[i] && first[i] != '\0') {
    ++i;
  }
  return static_cast<int>(first[i]) - static_cast<int>(second[i]);
}