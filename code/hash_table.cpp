#include "hash_table.h"

#include <algorithm>
#include <utility>

// == index ==
std::size_t HashTable::Capacity() const {
  return std::size_t{1} << bits_;
}

// Хеш-функція: множення зі зсувом (w = 32, m = 2^bits_)
std::size_t HashTable::Index(int key) const {
  std::uint32_t k = static_cast<std::uint32_t>(key);
  return (k * kMultiplier) >> (32 - bits_);
}

// == constructor ==
HashTable::HashTable(unsigned initial_bits)
    : buckets_(nullptr),
      bits_(std::clamp(initial_bits, kMinBits, kMaxBits)),
      count_(0) {
  buckets_ = new Node*[Capacity()]();   // () --- усі nullptr
}

// == search ==
const Row* HashTable::Search(int key) const {
  Node* current_node = buckets_[Index(key)];
  while (current_node != nullptr && current_node->row.key != key) {
    current_node = current_node->next;
  }
  if (current_node == nullptr) {
    return nullptr;   // ключа немає
  }
  return &current_node->row;
}

// == delete ==
bool HashTable::Delete(int key) {
  Node*& head = buckets_[Index(key)];   // голова «свого» ланцюжка

  Node* previous_node = nullptr;
  Node* current_node = head;
  while (current_node != nullptr && current_node->row.key != key) {
    previous_node = current_node;
    current_node = current_node->next;
  }
  if (current_node == nullptr) {
    return false;   // ключа немає
  }

  if (previous_node == nullptr) {
    head = current_node->next;   // вилучаємо голову
  } else {
    previous_node->next = current_node->next;
  }
  delete current_node;
  --count_;
  return true;
}

// == insert ==
bool HashTable::Insert(const Row& row) {
  Node*& head = buckets_[Index(row.key)];

  for (Node* n = head; n != nullptr; n = n->next) {
    if (n->row.key == row.key) {   // ключ уже є: замінюємо тіло
      n->row.value = row.value;
      return true;
    }
  }
  head = new Node{row, head};   // новий вузол --- голова
  ++count_;

  if (LoadFactor() > 1.0 && bits_ < kMaxBits) {
    Rehash();
  }
  return true;
}

// == rehash ==
void HashTable::Rehash() {
  Node** old_buckets = buckets_;
  std::size_t old_capacity = Capacity();

  ++bits_;   // m збільшується вдвічі
  buckets_ = new Node*[Capacity()]();

  for (std::size_t i = 0; i < old_capacity; ++i) {
    Node* n = old_buckets[i];
    while (n != nullptr) {
      Node* next = n->next;
      Node*& head = buckets_[Index(n->row.key)];   // нова адреса
      n->next = head;   // вузол не копіюємо,
      head = n;         // а лише перечіплюємо
      n = next;
    }
  }
  delete[] old_buckets;
}

// == rest ==
bool HashTable::IsEmpty() const {
  return count_ == 0;
}

double HashTable::LoadFactor() const {
  return static_cast<double>(count_) / Capacity();
}

void HashTable::Clear() {
  for (std::size_t i = 0; i < Capacity(); ++i) {
    while (buckets_[i] != nullptr) {
      Node* to_delete = buckets_[i];
      buckets_[i] = to_delete->next;
      delete to_delete;
    }
  }
  count_ = 0;
}

HashTable::~HashTable() {
  Clear();
  delete[] buckets_;
}

// Глибока копія: ті самі m комірок, той самий порядок у ланцюжках
HashTable::HashTable(const HashTable& other)
    : buckets_(nullptr), bits_(other.bits_), count_(other.count_) {
  buckets_ = new Node*[Capacity()]();
  for (std::size_t i = 0; i < Capacity(); ++i) {
    Node** last_ptr = &buckets_[i];
    for (Node* n = other.buckets_[i]; n != nullptr; n = n->next) {
      *last_ptr = new Node{n->row, nullptr};
      last_ptr = &((*last_ptr)->next);
    }
  }
}

// Присвоєння через копію та обмін (copy-and-swap)
HashTable& HashTable::operator=(HashTable other) {
  std::swap(buckets_, other.buckets_);
  std::swap(bits_, other.bits_);
  std::swap(count_, other.count_);
  return *this;   // other знищить старі дані *this
}

std::ostream& operator<<(std::ostream& out, const HashTable& table) {
  out << "HashTable: n = " << table.count_
      << ", m = " << table.Capacity()
      << ", load factor = " << table.LoadFactor() << std::endl;
  for (std::size_t i = 0; i < table.Capacity(); ++i) {
    if (table.buckets_[i] == nullptr) {
      continue;
    }
    out << "[" << i << "]";
    for (HashTable::Node* n = table.buckets_[i]; n != nullptr;
         n = n->next) {
      out << " -> " << n->row.key << ": " << n->row.value;
    }
    out << std::endl;
  }
  return out;
}
