#include <iostream>

#include "hash_table.h"
#include "open_hash_table.h"

// Гетьмани; ключ --- рік обрання
const Row kHetmans[] = {
    {1648, "Хмельницький"}, {1657, "Виговський"},
    {1665, "Дорошенко"},    {1672, "Самойлович"},
    {1687, "Мазепа"},       {1708, "Скоропадський"},
    {1727, "Апостол"},      {1750, "Розумовський"},
};

int main() {
  HashTable t;   // m = 8
  t.Insert({1648, "Хмельницький"});
  t.Insert({1657, "Виговський"});
  t.Insert({1665, "Дорошенко"});
  t.Insert({1648, "Богдан Хмельницький"});   // 1648 є: нове тіло
  t.Delete(1657);

  std::cout << t;
  std::cout << t.Search(1665)->value << std::endl;   // Дорошенко

  // Усі гетьмани і ще один запис: n = 9 > m = 8, буде перехешування
  for (const Row& row : kHetmans) {
    t.Insert(row);
  }
  t.Insert({1659, "Юрій Хмельницький"});
  std::cout << t;

  OpenHashTable o;   // m = 11, лінійне зондування
  for (const Row& row : kHetmans) {
    o.Insert(row);
  }
  o.Delete(1727);
  std::cout << o.Search(1750)->value << std::endl;   // Розумовський
  o.Insert({1727, "Апостол"});
  std::cout << o;
}
