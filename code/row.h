#ifndef ROW_H_
#define ROW_H_

#include <string>

// Запис таблиці: ключ і тіло
struct Row {
  int key = 0;
  std::string value;
};

#endif  // ROW_H_
