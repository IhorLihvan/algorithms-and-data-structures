#ifndef OPEN_HASH_TABLE_H_
#define OPEN_HASH_TABLE_H_

#include <iostream>
#include <vector>

#include "row.h"

// Хеш-таблиця з відкритою адресацією: ділення, лінійне зондування
class OpenHashTable {
 public:
  explicit OpenHashTable(int m = 11);   // m < 1 замінюється на 1

  bool Insert(const Row& row);   // false, якщо таблицю переповнено
  const Row* Search(int key) const;
  bool Delete(int key);
  bool IsEmpty() const;
  void Clear();

  friend std::ostream& operator<<(std::ostream& out,
                                  const OpenHashTable& table);

 private:
  enum class State { kEmpty, kFull, kDeleted };

  struct Slot {
    State state = State::kEmpty;
    Row row;
  };

  int H1(int key) const;
  int Probe(int key, int i) const;
  int FindSlot(int key) const;

  std::vector<Slot> slots_;   // m комірок, жодних вказівників
  int count_ = 0;             // n --- кількість записів
};

#endif  // OPEN_HASH_TABLE_H_
