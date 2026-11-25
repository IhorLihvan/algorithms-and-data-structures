#include <iostream>

#include "binary_search.h"
#include "direct_table.h"
#include "table.h"

int main() {
  Table t;
  t.Insert({1648, "Хмельницький"});
  t.Insert({1657, "Виговський"});
  t.Insert({1665, "Дорошенко"});
  t.Insert({1648, "Богдан Хмельницький"});   // 1648 є: нове тіло
  t.Delete(1657);

  std::cout << t;
  std::cout << t.Search(1665)->value << std::endl;   // Дорошенко
  bool absent = t.Search(1657) == nullptr;            // ключ вилучено
  std::cout << std::boolalpha << absent << std::endl;

  const Row hetmans[] = {
      {1648, "Хмельницький"}, {1657, "Виговський"},
      {1665, "Дорошенко"},    {1672, "Самойлович"},
      {1687, "Мазепа"},       {1708, "Скоропадський"},
      {1727, "Апостол"},      {1750, "Розумовський"},
  };
  int i = BinarySearch(hetmans, 8, 1727);
  std::cout << i << ": " << hetmans[i].value << std::endl;

  static DirectTable d;   // 676 записів: краще не на стеку
  d.Insert({TwoLetterKey("KN"), "Комп'ютерні науки"});
  d.Insert({TwoLetterKey("IT"), "Інформаційні технології"});
  std::cout << d;
  std::cout << TwoLetterKey("kn") << std::endl;   // -1: малі літери
}
