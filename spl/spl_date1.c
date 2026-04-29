#define _GNU_SOURCE
#include "common_hdrs.h"

int main() {
  char s[MAXLEN];
  char *format = "%c";
  time_t t = time(NULL);
  struct tm *t_tm = localtime(&t);
  if (t_tm == NULL) {
    fatal_error(EOVERFLOW, "strftime failed");
  };
  size_t result = strftime(s, sizeof(s), format, t_tm);
  if (result == 0) {
    fatal_error(EXIT_FAILURE, "strftime failed");
  };
  printf("[TIME]\n%s", s);
  return 0;
}
