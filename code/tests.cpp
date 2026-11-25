// Тести для всіх класів розділів «Таблиця» і «Хеш-таблиця».
// Кожна таблиця порівнюється зі std::map на випадкових операціях.
// Програма повертає 0, якщо всі перевірки пройшли.

#include <iostream>
#include <map>
#include <random>
#include <string>

#include "binary_search.h"
#include "direct_table.h"
#include "hash_string.h"
#include "hash_table.h"
#include "open_hash_table.h"
#include "table.h"

#ifdef _MSC_VER
#include <crtdbg.h>   // звіт про витоки пам'яті в налагоджувальній збірці
#endif

namespace {

int failures = 0;

void Check(bool condition, const char* what, int line) {
  if (!condition) {
    ++failures;
    std::cout << "FAIL (line " << line << "): " << what << std::endl;
  }
}

#define CHECK(condition) Check((condition), #condition, __LINE__)

// Перевіряє, що таблиця містить рівно ті записи, що й reference
template <typename T>
void CheckSame(const T& table, const std::map<int, std::string>& reference,
               int min_key, int max_key) {
  for (int k = min_key; k <= max_key; ++k) {
    const Row* row = table.Search(k);
    auto it = reference.find(k);
    if (it == reference.end()) {
      CHECK(row == nullptr);
    } else {
      CHECK(row != nullptr && row->key == k && row->value == it->second);
    }
  }
  CHECK(table.IsEmpty() == reference.empty());
}

// Випадкові вставки, пошуки й видалення; capacity обмежує кількість
// записів для таблиць, які можуть заповнитися
template <typename T>
void RandomTest(T& table, int min_key, int max_key, std::size_t capacity) {
  std::mt19937 rng(42);
  std::uniform_int_distribution<int> key(min_key, max_key);
  std::uniform_int_distribution<int> op(0, 2);
  std::map<int, std::string> reference;
  for (int step = 0; step < 5000; ++step) {
    int k = key(rng);
    std::string v = std::to_string(step);
    int operation = op(rng);
    if (operation == 0) {
      if (reference.size() < capacity || reference.count(k) > 0) {
        CHECK(table.Insert({k, v}));
        reference[k] = v;
      }
    } else if (operation == 1) {
      CHECK(table.Delete(k) == (reference.erase(k) == 1));
    } else {
      const Row* row = table.Search(k);
      CHECK((row != nullptr) == (reference.count(k) == 1));
    }
  }
  CheckSame(table, reference, min_key, max_key);

  T copy(table);                  // конструктор копіювання
  CheckSame(copy, reference, min_key, max_key);
  T assigned;
  assigned.Insert({min_key, "old"});
  assigned = table;               // присвоєння непорожній таблиці
  CheckSame(assigned, reference, min_key, max_key);
  T& alias = assigned;
  assigned = alias;               // присвоєння самій собі
  CheckSame(assigned, reference, min_key, max_key);

  table.Clear();
  CHECK(table.IsEmpty());
  CheckSame(copy, reference, min_key, max_key);   // копія незалежна
}

void TestTable() {
  Table t;
  CHECK(t.IsEmpty());
  CHECK(t.Search(1648) == nullptr);
  CHECK(!t.Delete(1648));
  t.Insert({1648, "Хмельницький"});
  t.Insert({1648, "Богдан Хмельницький"});
  CHECK(t.Search(1648)->value == "Богдан Хмельницький");
  CHECK(t.Delete(1648));
  CHECK(t.IsEmpty());

  Table random;
  RandomTest(random, -50, 50, 1000);
}

void TestBinarySearch() {
  const Row rows[] = {{1648, ""}, {1657, ""}, {1665, ""}, {1672, ""},
                      {1687, ""}, {1708, ""}, {1727, ""}, {1750, ""}};
  for (int i = 0; i < 8; ++i) {
    CHECK(BinarySearch(rows, 8, rows[i].key) == i);
  }
  CHECK(BinarySearch(rows, 8, 1600) == -1);
  CHECK(BinarySearch(rows, 8, 1700) == -1);
  CHECK(BinarySearch(rows, 8, 1800) == -1);
  CHECK(BinarySearch(rows, 0, 1648) == -1);
}

void TestDirectTable() {
  CHECK(TwoLetterKey("AB") == 1);
  CHECK(TwoLetterKey("KN") == 273);
  CHECK(TwoLetterKey("ZZ") == 675);
  CHECK(TwoLetterKey("kn") == -1);
  CHECK(TwoLetterKey("K") == -1);
  CHECK(TwoLetterKey("KNN") == -1);
  CHECK(TwoLetterKey("K1") == -1);
  CHECK(TwoLetterKey("") == -1);

  static DirectTable d;
  CHECK(!d.Insert({-1, "x"}));
  CHECK(!d.Insert({DirectTable::kSize, "x"}));
  CHECK(d.Search(-1) == nullptr);
  CHECK(d.Search(DirectTable::kSize) == nullptr);
  CHECK(!d.Delete(-1));
  CHECK(d.IsEmpty());

  static DirectTable random;
  RandomTest(random, 0, DirectTable::kSize - 1, DirectTable::kSize);
}

void TestHashTable() {
  CHECK(HashString("pt") == 3588);

  HashTable zero(0);    // має обмежитися до m = 2, а не зсуву на 32
  HashTable huge(64);   // має обмежитися зверху
  for (int k = 0; k < 100; ++k) {
    zero.Insert({k, "x"});
  }
  CHECK(zero.Search(99) != nullptr);
  CHECK(zero.LoadFactor() <= 1.0);
  huge.Insert({1, "x"});
  CHECK(huge.Search(1) != nullptr);

  HashTable random(1);
  RandomTest(random, -200, 200, 1000);
}

void TestOpenHashTable() {
  OpenHashTable full(3);
  CHECK(full.Insert({1, "a"}));
  CHECK(full.Insert({2, "b"}));
  CHECK(full.Insert({3, "c"}));
  CHECK(!full.Insert({4, "d"}));   // переповнено
  CHECK(full.Insert({2, "e"}));    // заміна тіла можлива
  CHECK(full.Search(2)->value == "e");

  OpenHashTable one(0);   // m < 1 замінюється на 1
  CHECK(one.Insert({-7, "x"}));
  CHECK(one.Search(-7) != nullptr);

  OpenHashTable random(97);
  RandomTest(random, -150, 150, 97);
}

}  // namespace

int main() {
#ifdef _MSC_VER
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif
  TestTable();
  TestBinarySearch();
  TestDirectTable();
  TestHashTable();
  TestOpenHashTable();

  if (failures == 0) {
    std::cout << "All tests passed" << std::endl;
    return 0;
  }
  std::cout << failures << " checks failed" << std::endl;
  return 1;
}
