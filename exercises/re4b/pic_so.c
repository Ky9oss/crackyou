#include <stdio.h>

int global_variable = 123;

int f1(int var) {
  int rt = global_variable + var;
  printf("returning %d\n", rt);
  return rt;
}
