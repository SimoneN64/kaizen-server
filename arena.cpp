#include <arena.hpp>
#include <cstdint>

template <> uint8_t ArenaReadBuffer::Read() {
  uint8_t ret = *(uint8_t*)(buffer + cursor);
  cursor++;
  return ret;
}

template <> std::string ArenaReadBuffer::Read() {
  auto prev_cur = cursor;
  while ((buffer + cursor) != '\0') {
    cursor++;
  }

  std::string ret;
  ret.resize(cursor - prev_cur);
  memcpy(ret.data(), &buffer[prev_cur], cursor - prev_cur);
  return ret;
}