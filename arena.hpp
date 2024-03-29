#pragma once
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

struct ArenaBuffer {
  explicit ArenaBuffer(size_t maxSize = 0x1000) : maxSize(maxSize) {
    buffer = malloc(maxSize);
    memset(buffer, 0, maxSize);
  }

  ~ArenaBuffer() {
    free(buffer);
  }

  template<typename T>
  void Write(const T& v) {
    memcpy((uint8_t*)buffer + cursor, &v, sizeof(T));
    cursor += sizeof(T);
  }

  template<typename T>
  void Write(const std::vector<T>& v) {
    memcpy((uint8_t*)buffer + cursor, v.data(), sizeof(T)*v.size());
    cursor += sizeof(T)*v.size();
  }

  void Write(const std::string& s) {
    memcpy((uint8_t*)buffer + cursor, s.c_str(), s.length());
    cursor += s.length();
  }

  void Reset() { cursor = 0; }
  const void* GetBuffer() { return buffer; }
  [[nodiscard]] size_t GetSize() const { return cursor; }
private:
  void* buffer{};
  size_t maxSize=0;
  size_t cursor=0;
};

struct ArenaReadBuffer {
  ArenaReadBuffer(void* buffer, size_t size) : buffer(buffer), size(size) {}

  template <typename T> T Read();
private:
  void* buffer{};
  size_t size{};
  size_t cursor{};
};