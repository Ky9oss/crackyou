#include <stdio.h>
#include <stdlib.h>

// #ifndef RAND_MAX
// #undef RAND_MAX
// #define RAND_MAX 100
// #endif

int main() {
  int target = rand() % 100;
  printf("The random target is: %d\n", target);

  int guess = 0;
  int start = 0;
  int end = 99;

  int i = 0;

  while (1) {
    guess = start + (end - start) / 2;

    if (guess < target) {
      start = guess + 1;
    } else if ( guess > target ){
      end = guess;
    } else {
        break;
    };

    i++;
    if (i > 10000) {
      printf("Error: no result\n");
      return 1;
    };
  };

  printf("Found the target %d in %d steps.\n", guess, i);

  return 0;
}
