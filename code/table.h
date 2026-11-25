#ifndef TABLE_H_
#define TABLE_H_

#include <iostream>

#include "row.h"

// Невпорядкована таблиця на однозв'язному списку
class Table {
 public:
  Table() = default;
  Table(const Table& other);
  Table& operator=(Table other);
  ~Table();

  bool Insert(const Row& row);
  const Row* Search(int key) const;
  bool Delete(int key);
  bool IsEmpty() const;
  void Clear();

  friend std::ostream& operator<<(std::ostream& out,
                                  const Table& table);

 private:
  // Вузол списку; зовні класу не видно
  struct Node {
    Row row;
    Node* next;
  };

  Node* head_ = nullptr;   // перший вузол (голова списку)
};

#endif  // TABLE_H_
