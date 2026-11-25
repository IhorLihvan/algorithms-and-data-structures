#ifndef BINARY_SEARCH_H_
#define BINARY_SEARCH_H_

#include "row.h"

// Повертає індекс запису з ключем key або -1, якщо його немає.
// rows[0..n-1] відсортовано за зростанням key.
int BinarySearch(const Row* rows, int n, int key);

#endif  // BINARY_SEARCH_H_
