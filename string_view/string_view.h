#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <cstddef>
#include <cstring>

class StringView {
 private:
  const char* string_{};
  std::size_t size_{};

 public:
  StringView();
  StringView(const char*);  // NOLINT
  StringView(const char*, std::size_t);

  const char& operator[](std::size_t) const;
  const char& At(std::size_t) const;
  const char& Front() const;
  const char& Back() const;

  std::size_t Size() const;
  std::size_t Length() const;

  bool Empty() const;

  const char* Data() const;

  void Swap(StringView&);
  void RemovePrefix(std::size_t);
  void RemoveSuffix(std::size_t);

  StringView Substr(std::size_t, std::size_t) const;
};

class StringViewOutOfRange {};

#endif
