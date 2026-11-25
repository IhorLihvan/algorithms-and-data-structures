#include "binary_search.h"

int BinarySearch(const Row* rows, int n, int key) {
  int left = 0;
  int right = n - 1;
  while (left <= right) {
    int mid = left + (right - left) / 2;   // без переповнення int
    if (rows[mid].key == key) {
      return mid;
    }
    if (rows[mid].key < key) {
      left = mid + 1;    // шукаємо праворуч
    } else {
      right = mid - 1;   // шукаємо ліворуч
    }
  }
  return -1;
}
