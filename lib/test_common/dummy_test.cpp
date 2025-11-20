#include "dummy_test.h"

#ifdef PIO_UNIT_TESTING
int add(int a, int b) {
  return a + b;
}

bool isEven(int n) {
  return (n % 2) == 0;
}
#endif
