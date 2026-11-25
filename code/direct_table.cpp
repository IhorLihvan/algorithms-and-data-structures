#include "direct_table.h"

// == key ==
int TwoLetterKey(const std::string& s) {
  if (s.size() != 2) {
    return -1;
  }
  for (char c : s) {
    if (c < 'A' || c > 'Z') {
      return -1;   // лише великі латинські літери
    }
  }
  return (s[0] - 'A') * 26 + (s[1] - 'A');
}

// == operations ==
bool DirectTable::IsValidKey(int key) {
  return 0 <= key && key < kSize;
}

bool DirectTable::Insert(const Row& row) {
  if (!IsValidKey(row.key)) {
    return false;   // ключ поза універсумом
  }
  if (!used_[row.key]) {
    used_[row.key] = true;
    ++count_;
  }
  rows_[row.key] = row;
  return true;
}

const Row* DirectTable::Search(int key) const {
  if (!IsValidKey(key) || !used_[key]) {
    return nullptr;
  }
  return &rows_[key];
}

bool DirectTable::Delete(int key) {
  if (!IsValidKey(key) || !used_[key]) {
    return false;
  }
  used_[key] = false;
  --count_;
  return true;
}

// == rest ==
bool DirectTable::IsEmpty() const {
  return count_ == 0;
}

void DirectTable::Clear() {
  for (int k = 0; k < kSize; ++k) {
    used_[k] = false;
  }
  count_ = 0;
}

std::ostream& operator<<(std::ostream& out, const DirectTable& table) {
  out << "DirectTable:" << std::endl;
  for (int k = 0; k < DirectTable::kSize; ++k) {
    if (table.used_[k]) {
      out << k << ": " << table.rows_[k].value << std::endl;
    }
  }
  return out;
}
