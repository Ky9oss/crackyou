#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static char *c = "fuck world";

void arithmetic_types();

void swap(int *, int *);
void print_enum(); // Prior to C23, print_enum accept any number of arguments of
                   // any type. Starting with C23 and always in C++, print_enum
                   // takes no argument

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

// pass by value (default) vs pass by reference
void swap(int *pa, int *pb) {
  int t = *pa;
  *pa = *pb;
  *pb = t;
  printf("swap: a = %d, b = %d\n", *pa, *pb);
}

void getting_start() {
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
  //    str[11] means 11 elements. But the last one is str[10]. This can be
  //    confusing. str is the pointer to the first member of the array str[i] =
  //    *(str + i) &str[i] = str + i
  char str[11];
  for (unsigned int i = 0; i < 10; ++i) {
    str[i] = '0' + i;
  }
  str[10] = '\0';

  printf("%s", c);
}

int main() {

  arithmetic_types();

  return EXIT_SUCCESS;
}

#define TYPEOF(x)                                                              \
  _Generic((x),                                                                \
      int: "signed int",                                                       \
      short: "short",                                                          \
      unsigned short: "unsinged short",                                        \
      long: "signed long",                                                     \
      long long: "signed long long",                                           \
      unsigned long: "unsigned long",                                          \
      unsigned long long: "unsigned long long",                                \
      double: "double",                                                        \
      unsigned int: "unsigned int",                                            \
      default: "other")

void arithmetic_types() {
  int mask = 0b110011; // start with C23
  unsigned _BitInt(31) s = 17;
  double x = 0.23 * pow(2.0, DBL_MIN_EXP);

  const char *show_classification(double x) {
    switch (fpclassify(x)) {
    case FP_INFINITE:
      return "Inf";
    case FP_NAN:
      return "NaN";
    case FP_NORMAL:
      return "normal";
    case FP_SUBNORMAL:
      return "subnormal";
    case FP_ZERO:
      return "zero";
    default:
      return "unknown";
    }
  };

  float b = 25E-4;
  printf("%f\n", b);

  char *a = show_classification(x);
  printf("%s\n", a);
  {
    unsigned int m = 5;
    int n = -10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    // Usual Arithmetic Conversions:
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }

  // printf("m + n: %u\n", m + n);

  {
    double m = 5.0;
    int n = -10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    // Usual Arithmetic Conversions:
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }
  {

    unsigned int m = 5;
    signed long n = 10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    // Usual Arithmetic Conversions:
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }

  {
    unsigned int m = 5;
    long long n = 10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    // Usual Arithmetic Conversions:
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }

  {
    short m = 5;
    short n = 10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }

  {
    unsigned short m = 5;
    short n = 10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }

  {
    unsigned short m = 5;
    unsigned short n = 10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    // Integer Promotions: value-preserving approach
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }

  {
    unsigned short m = pow(2, 16) - 5;
    unsigned short n = pow(2, 16) - 10;
    char *m_type = TYPEOF(m);
    char *n_type = TYPEOF(n);
    // Integer Promotions: value-preserving approach
    printf("m(%s) + n(%s): %s\n", m_type, n_type, TYPEOF(m + n));
  }

  int i = 5;
  printf("Result = %d\n", i++ * i++);

  int ii;
  size_t ii_size = sizeof i;
  size_t int_size = sizeof(int);
  size_t short_size = sizeof(short);
  size_t bitint7_size = sizeof(_BitInt(7));
  size_t bitint4_size = sizeof(_BitInt(4));
  size_t bitint12_size = sizeof(_BitInt(12));
  size_t bitint9_size = sizeof(_BitInt(9));

  printf("ii_size = %u\n", ii_size);
  printf("int_size = %u\n", int_size);
  printf("short_size = %u\n", short_size);
  printf("bitint7_size = %u\n", bitint7_size);
  printf("bitint4_size = %u\n", bitint4_size);
  printf("bitint12_size = %u\n", bitint12_size);
  printf("bitint9_size = %u\n", bitint9_size);

  printf("!15 = %d\n", !15);
  printf("!\"1234\" = %d\n", !"1234");
  printf("!0 = %d\n", !0);
  printf("!nullptr = %d\n", !nullptr);
  printf("!NULL = %d\n", !NULL);

}
