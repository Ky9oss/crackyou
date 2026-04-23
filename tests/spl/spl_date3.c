/* 
 *
 * spl_date3 [-h] | [-d "time-adjustment"] [+"format-specification"]
 *
 * time-adjustment = <num> <time-unit> [<num> <time-unit> ... ]
 * num = [+|-]<integer>
 * time-unit = year[s] month[s] week[s] day[s] hour[s] minute[s] second[s]
 * integer = [1-9][0-9].. 
 * */

#define _GNU_SOURCE
#include "common_hdrs.h"

void getformat(int nargs, char *argvec[], char *format_str) {
  // printf("%d", argvec[nargs - 1][0]);
  if (argvec[nargs - 1][0] == '+')
    if (strlen(argvec[nargs - 1]) + 1 < MAXLEN)
      strncpy(format_str, argvec[nargs - 1] + 1, MAXLEN - 1);
    else
      fatal_error(BAD_FORMAT_ERROR, "The format string length is too long");
  else
    fatal_error(BAD_FORMAT_ERROR, "Format should be +\"format-string\"");
}
int main(int argc, char *argv[]) {
  char s[MAXLEN];
  char format[MAXLEN];

  if (argc < 2)
    strcpy(format, "%c");
  else
    getformat(argc, argv, format);

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
