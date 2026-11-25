#include "hash_string.h"

std::uint32_t HashString(const std::string& s) {
  std::uint32_t h = 0;
  for (unsigned char c : s) {
    h = h * 31 + c;   // переповнення = взяття за mod 2^32
  }
  return h;
}
