#ifndef DIRECT_TABLE_H_
#define DIRECT_TABLE_H_

#include <iostream>
#include <string>

#include "row.h"

// Ключ із рядка з двох великих латинських літер:
// "KN" -> 10 * 26 + 13 = 273. Повертає -1 для будь-якого іншого рядка.
int TwoLetterKey(const std::string& s);

// Таблиця з прямим доступом для ключів 0..kSize-1
class DirectTable {
 public:
  static constexpr int kSize = 26 * 26;   // 676 можливих ключів

  bool Insert(const Row& row);
  const Row* Search(int key) const;
  bool Delete(int key);
  bool IsEmpty() const;
  void Clear();

  friend std::ostream& operator<<(std::ostream& out,
                                  const DirectTable& table);

 private:
  static bool IsValidKey(int key);

  Row rows_[kSize];         // запис із ключем k --- у комірці k
  bool used_[kSize] = {};   // чи зайнята комірка
  int count_ = 0;           // кількість записів
};

#endif  // DIRECT_TABLE_H_
