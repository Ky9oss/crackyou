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

// struct tm
// {
//   int tm_sec;			/* Seconds.	[0-60] (1 leap second)
//   */ int tm_min;			/* Minutes.	[0-59] */ int tm_hour;
//   /* Hours.	[0-23] */ int tm_mday;			/* Day.		[1-31]
//   */ int tm_mon;			/* Month.	[0-11] */ int tm_year;
//   /* Year	- 1900.  */
//   ...

int parse_time_adjustment(char *time_adjustment_string, struct tm *datetm) {
  char *delim = " \t";
  char *next_token;
  int flags = ONLY_DIGITS | NO_TRAILING;
  int number;
  char err_msg[MAXLEN];
  int res;

  next_token = strtok(time_adjustment_string, delim);
  // printf("first token: %s\n", next_token);
  while (next_token != NULL) {
    // next_token = strtok(time_adjustment_string, delim);
    res = get_int(next_token, flags, &number, err_msg);
    if (res != VALID_NUMBER) {
      fprintf(stderr, "get_int failed: %s\n", err_msg);
      fprintf(stderr, "Usage: [-h] | [-d \"time-adjustment\"] "
                      "[+\"format-specification\"]\n");
      exit(EXIT_FAILURE);
    };

    next_token = strtok(NULL, delim);
    // printf("next_token: %s\n", next_token);

    if (next_token == NULL) {
      fprintf(stderr, "next token == null\n");
      fprintf(stderr, "Usage: [-h] | [-d \"time-adjustment\"] "
                      "[+\"format-specification\"]\n");
      exit(EXIT_FAILURE);
    };

    if (strstr(next_token, "year") != NULL) {
      datetm->tm_year += number;
    } else if (strstr(next_token, "month") != NULL) {
      datetm->tm_mon += number;
    } else if (strstr(next_token, "week") != NULL) {
      datetm->tm_mday += number * 7;
    } else if (strstr(next_token, "day") != NULL) {
      datetm->tm_mday += number;
    } else if (strstr(next_token, "hour") != NULL) {
      datetm->tm_hour += number;
    } else if (strstr(next_token, "minute") != NULL) {
      datetm->tm_min += number;
    } else if (strstr(next_token, "second") != NULL) {
      datetm->tm_sec += number;
    }

    next_token = strtok(NULL, delim);

  };

  return 0;
}

int main(int argc, char *argv[]) {
  int opt;

  char s[MAXLEN];
  char format[MAXLEN];
  char optarg_buffer[MAXLEN];

  time_t t = time(NULL);
  struct tm *t_tm = localtime(&t);
  if (t_tm == NULL) {
    fatal_error(EOVERFLOW, "strftime failed");
  };

  while ((opt = getopt(argc, argv, ":hd:")) != -1) {
    switch (opt) {
    case 'd':
      strcpy(optarg_buffer, optarg);
      // printf("optarg: %s\n", optarg_buffer);
      parse_time_adjustment(optarg_buffer, t_tm);
      break;
    case 'h':
      fprintf(stderr,
              "Usage: %s [-h] | [-d \"time-adjustment\"] "
              "[+\"format-specification\"]\n",
              argv[0]);
      exit(EXIT_SUCCESS);
      break;
    case '?':
      printf("Option is not supported");
      fprintf(stderr,
              "Usage: %s [-h] | [-d \"time-adjustment\"] "
              "[+\"format-specification\"]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    case ':':
      printf("Option has a missing argument");
      fprintf(stderr,
              "Usage: %s [-h] | [-d \"time-adjustment\"] "
              "[+\"format-specification\"]\n",
              argv[0]);
      exit(EXIT_FAILURE);
    default: /* '?' */
      break;
    }
  }

  if (optind >= argc) {
    strcpy(format, "%c");
  } else {
    getformat(argc, argv, format);
  };

  size_t result = strftime(s, sizeof(s), format, t_tm);
  if (result == 0) {
    fatal_error(EXIT_FAILURE, "strftime failed");
  };
  printf("[TIME]\n%s", s);
  return 0;
}
