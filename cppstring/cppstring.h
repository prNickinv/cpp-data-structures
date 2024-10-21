#ifndef CPPSTRING_H
#define CPPSTRING_H

#include <iostream>
#include <stdexcept>

class StringOutOfRange : public std::out_of_range {
 public:
  StringOutOfRange() : std::out_of_range("StringOutOfRange") {
  }
};

class String {
 private:
  char* string_{};
  std::size_t size_{};
  std::size_t capacity_{};

 public:
  String();
  String(std::size_t, char);
  String(const char*);  // NOLINT
  String(const char*, std::size_t);

  String(const String&);
  String& operator=(const String&);
  ~String();

  const char& operator[](std::size_t) const;
  char& operator[](std::size_t);
  [[nodiscard]] const char& At(std::size_t) const;
  char& At(std::size_t);

  [[nodiscard]] const char& Front() const;
  char& Front();
  [[nodiscard]] const char& Back() const;
  char& Back();

  [[nodiscard]] const char* CStr() const;
  char* CStr();
  [[nodiscard]] const char* Data() const;

  [[nodiscard]] bool Empty() const;

  [[nodiscard]] std::size_t Size() const;
  [[nodiscard]] std::size_t Length() const;
  [[nodiscard]] std::size_t Capacity() const;

  void Clear();
  void Swap(String&);
  void PopBack();
  void PushBack(char);

  String& operator+=(const String&);
  void Resize(std::size_t, char);

  void Reserve(std::size_t);
  void ShrinkToFit();
  String operator+(const String&) const;
  String operator+(const char*) const;
  friend String operator+(const char*, const String&);

  bool operator==(const String&) const;
  bool operator!=(const String&) const;
  bool operator<(const String&) const;
  bool operator<=(const String&) const;
  bool operator>(const String&) const;
  bool operator>=(const String&) const;

  friend std::ostream& operator<<(std::ostream&, const String&);
  friend std::istream& operator>>(std::istream&, String);

  static std::size_t Strlen(const char*);
  static int Strcmp(const char*, const char*);
};

#endif
