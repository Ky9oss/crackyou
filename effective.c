#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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

  // nullptr: Starting with C23
  // nullptr use to replace NULL because NULL have different value in different
  // type:
  //
  // NULL
  // value      type
  // 0          int
  // 0L         long
  // (void *)0  void *
  //
  // Error:
  //    (true ? 1 : NULL) is a constraint violation if NULL has type void *.
  //    123
  //
  void *vp = nullptr;

  // Array
  //    str[11] means 11 elements. But the last one is str[10]. This can be confusing.
  //    str is the pointer to the first member of the array
  //    str[i] = *(str + i)
  //    &str[i] = str + i
  char str[11];
  for (unsigned int i = 0; i < 10; ++i) {
    str[i] = '0' + i;
  }
  str[10] = '\0';

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
