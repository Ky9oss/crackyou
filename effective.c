#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

void swap(int *, int *);
void print_enum(); // Prior to C23, print_enum accept any number of arguments of
                   // any type. Starting with C23 and always in C++, print_enum
                   // takes no argument

// pass by value (default) vs pass by reference
void swap(int *pa, int *pb) {
  int t = *pa;
  *pa = *pb;
  *pb = t;
  printf("swap: a = %d, b = %d\n", *pa, *pb);
}

int main() {
  int a = 22;
  int b = 16;
  swap(&a, &b);

  printf("main: a = %d, b = %d\n", a, b);

  // Starting with C23 and always in C++
  print_enum();

  void *vp = nullptr;

  return EXIT_SUCCESS;
}

// Prior to C23
// void print_enum(int sun, int mon ,int tue, int wed) {
//   printf("enum: sun = %d, mon = %d, tue = %d, wed = %d", sun, mon, tue, wed);
// }
//
// enum day { sun, mon, tue, wed, thu, fri, sat };
// print_enum(sun, mon ,tue ,wed);

// Starting with C23 and always in C++
void print_enum() {
  enum day { sun, mon, tue, wed, thu, fri, sat };
  printf("enum: sun = %d, mon = %d, tue = %d, wed = %d", sun, mon, tue, wed);
}
