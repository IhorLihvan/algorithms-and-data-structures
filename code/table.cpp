#include "table.h"

#include <utility>

// == search ==
const Row* Table::Search(int key) const {
  Node* current_node = head_;
  while (current_node != nullptr && current_node->row.key != key) {
    current_node = current_node->next;
  }
  if (current_node == nullptr) {
    return nullptr;   // ключа немає
  }
  return &current_node->row;
}

// == delete ==
bool Table::Delete(int key) {
  Node* previous_node = nullptr;
  Node* current_node = head_;
  while (current_node != nullptr && current_node->row.key != key) {
    previous_node = current_node;
    current_node = current_node->next;
  }
  if (current_node == nullptr) {
    return false;   // ключа немає
  }

  if (previous_node == nullptr) {
    head_ = current_node->next;                 // вилучаємо голову
  } else {
    previous_node->next = current_node->next;   // «перешиваємо» список
  }
  delete current_node;
  return true;
}

// == insert ==
bool Table::Insert(const Row& row) {
  Delete(row.key);                // ключ має бути унікальним
  head_ = new Node{row, head_};   // новий вузол стає головою
  return true;
}

// == rest ==
bool Table::IsEmpty() const {
  return head_ == nullptr;
}

void Table::Clear() {
  while (head_ != nullptr) {
    Node* to_delete = head_;
    head_ = head_->next;
    delete to_delete;
  }
}

Table::~Table() {
  Clear();
}

// Глибока копія: нові вузли з тими самими записами, той самий порядок
Table::Table(const Table& other) {
  Node** last_ptr = &head_;   // поле, куди записати адресу наступного
  for (Node* n = other.head_; n != nullptr; n = n->next) {
    *last_ptr = new Node{n->row, nullptr};
    last_ptr = &((*last_ptr)->next);
  }
}

// Присвоєння через копію та обмін (copy-and-swap)
Table& Table::operator=(Table other) {
  std::swap(head_, other.head_);
  return *this;   // other знищить старі вузли *this
}

std::ostream& operator<<(std::ostream& out, const Table& table) {
  out << "Table:" << std::endl;
  for (Table::Node* n = table.head_; n != nullptr; n = n->next) {
    out << n->row.key << ": " << n->row.value << std::endl;
  }
  return out;
}
