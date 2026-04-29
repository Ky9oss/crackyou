// Print args
//
// Author: Ky9oss

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

static int cmpstringp(const void *p1, const void *p2) {
  /* The actual arguments to this function are "pointers to
     pointers to char", but strcmp(3) arguments are "pointers
     to char", hence the following cast plus dereference. */

  return strcmp(*(const char **)p1, *(const char **)p2);
}

int main(int argc, char *argv[]) {
  int opt;
  int reverse_flag = FALSE;
  int sort_flag = FALSE;
  long seq_start = -1;
  long seq_end = -1;

  char *endptr;
  int base = 10;

  while ((opt = getopt(argc, argv, ":rts:e:")) != -1) {
    switch (opt) {
    case 'r':
      reverse_flag = TRUE;
      break;
    case 't':
      sort_flag = TRUE;
      break;
    case 's':
      seq_start = strtol(optarg, &endptr, base);
      if (errno == EINVAL) {
        printf("The given base contains an unsportted value");
        exit(EXIT_FAILURE);
      } else if (errno == ERANGE) {
        printf("The resulting value is out of range");
        exit(EXIT_FAILURE);
      } else if (strcmp(endptr, "\0") != 0) {
        printf("Warning: The argument has some non-digital value which would "
               "not convert to digit\n");
      };
      // printf("%ld", seq_start);
      break;
    case 'e':
      seq_end = strtol(optarg, &endptr, base);
      if (errno == EINVAL) {
        printf("The given base contains an unsportted value");
        exit(EXIT_FAILURE);
      } else if (errno == ERANGE) {
        printf("The resulting value is out of range");
        exit(EXIT_FAILURE);
      } else if (strcmp(endptr, "\0") != 0) {
        printf("Warning: The argument has some non-digital value which would "
               "not convert to digit\n");
      };
      // printf("%ld", seq_end);
      break;
    case '?':
      printf("Option is not supported");
      exit(EXIT_FAILURE);
    case ':':
      printf("Option has a missing argument");
      exit(EXIT_FAILURE);
    default:
      break;
    }
  };

  if (seq_start > 0 && seq_end > 0 && seq_start <= seq_end) {
    for (long i = seq_start; i <= seq_end; i++) {
      printf("%ld\n", i);
    }
    exit(EXIT_SUCCESS);
  } else if (optind >= argc) {
    fprintf(stderr, "Expected argument after options\n");
    exit(EXIT_FAILURE);
  };

  if (sort_flag) {

    qsort(&argv[optind], argc - optind, sizeof(char *), cmpstringp);
  }

  if (reverse_flag) {
    for (int i = argc - 1; i >= optind; i--) {
      printf("%s\n", argv[i]);
    };
  } else {
    for (int i = optind; i < argc; i++) {
      printf("%s\n", argv[i]);
    };
  }
}
