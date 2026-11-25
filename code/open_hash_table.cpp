#include "open_hash_table.h"

#include <algorithm>

// == helpers ==
OpenHashTable::OpenHashTable(int m) : slots_(std::max(m, 1)) {
  // усі комірки спочатку мають стан kEmpty
}

// Метод ділення; залишок невід'ємний і для key < 0
int OpenHashTable::H1(int key) const {
  int m = static_cast<int>(slots_.size());
  return ((key % m) + m) % m;
}

// Лінійне зондування: h(k, i) = (h1(k) + i) mod m
int OpenHashTable::Probe(int key, int i) const {
  return (H1(key) + i) % static_cast<int>(slots_.size());
}

// Номер комірки з ключем key або -1
int OpenHashTable::FindSlot(int key) const {
  int m = static_cast<int>(slots_.size());
  for (int i = 0; i < m; ++i) {
    const Slot& s = slots_[Probe(key, i)];
    if (s.state == State::kEmpty) {
      return -1;   // далі ключа бути не може
    }
    if (s.state == State::kFull && s.row.key == key) {
      return Probe(key, i);
    }
    // kDeleted --- комірку пропускаємо і йдемо далі
  }
  return -1;
}

// == operations ==
const Row* OpenHashTable::Search(int key) const {
  int q = FindSlot(key);
  if (q == -1) {
    return nullptr;
  }
  return &slots_[q].row;
}

bool OpenHashTable::Insert(const Row& row) {
  int q = FindSlot(row.key);
  if (q != -1) {   // ключ уже є: замінюємо тіло
    slots_[q].row.value = row.value;
    return true;
  }
  int m = static_cast<int>(slots_.size());
  for (int i = 0; i < m; ++i) {   // перша комірка kEmpty або kDeleted
    Slot& s = slots_[Probe(row.key, i)];
    if (s.state != State::kFull) {
      s = Slot{State::kFull, row};
      ++count_;
      return true;
    }
  }
  return false;
}

bool OpenHashTable::Delete(int key) {
  int q = FindSlot(key);
  if (q == -1) {
    return false;
  }
  // Не kEmpty, інакше «обірвемо» пошук інших ключів
  slots_[q].state = State::kDeleted;
  slots_[q].row = Row{};
  --count_;
  return true;
}

// == rest ==
bool OpenHashTable::IsEmpty() const {
  return count_ == 0;
}

void OpenHashTable::Clear() {
  for (Slot& s : slots_) {
    s = Slot{};
  }
  count_ = 0;
}

std::ostream& operator<<(std::ostream& out,
                         const OpenHashTable& table) {
  for (int q = 0; q < static_cast<int>(table.slots_.size()); ++q) {
    out << q << ":";
    const OpenHashTable::Slot& s = table.slots_[q];
    if (s.state == OpenHashTable::State::kFull) {
      out << " " << s.row.key;
    } else if (s.state == OpenHashTable::State::kDeleted) {
      out << " DELETED";
    }
    out << std::endl;
  }
  return out;
}
