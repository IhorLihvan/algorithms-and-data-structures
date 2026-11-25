#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <cstddef>
#include <cstdint>
#include <iostream>

#include "row.h"

// Хеш-таблиця з ланцюжками; інтерфейс той самий, що в Table
class HashTable {
 public:
  // m = 2^initial_bits, де initial_bits обмежено [kMinBits, kMaxBits]
  explicit HashTable(unsigned initial_bits = 3);
  HashTable(const HashTable& other);
  HashTable& operator=(HashTable other);
  ~HashTable();

  bool Insert(const Row& row);
  const Row* Search(int key) const;
  bool Delete(int key);
  bool IsEmpty() const;
  void Clear();
  double LoadFactor() const;

  friend std::ostream& operator<<(std::ostream& out,
                                  const HashTable& table);

 private:
  // Вузол ланцюжка; зовні класу не видно
  struct Node {
    Row row;
    Node* next;
  };

  // Зсув на 32 - bits_ в Index визначений лише для 1 <= bits_ <= 31
  static constexpr unsigned kMinBits = 1;
  static constexpr unsigned kMaxBits = 30;
  // floor(A * 2^32), де A = (sqrt(5) - 1) / 2
  static constexpr std::uint32_t kMultiplier = 2654435769u;

  std::size_t Capacity() const;
  std::size_t Index(int key) const;
  void Rehash();

  Node** buckets_;      // масив з m вказівників на голови ланцюжків
  unsigned bits_;       // m = 2^bits_
  std::size_t count_;   // n --- кількість записів
};

#endif  // HASH_TABLE_H_
