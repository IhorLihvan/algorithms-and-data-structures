#ifndef HASH_STRING_H_
#define HASH_STRING_H_

#include <cstdint>
#include <string>

// Рядок як число в системі з основою 31 (схема Горнера), mod 2^32
std::uint32_t HashString(const std::string& s);

#endif  // HASH_STRING_H_
