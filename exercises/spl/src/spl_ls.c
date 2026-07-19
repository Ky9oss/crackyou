#define _GNU_SOURCE
#include "common_hdrs.h"
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>

// Task1: Modify the spl_ls1.c program so that it does not display the . and
// .. entries and sorts the entries in the collating order of the current
// locale. You’ll need an array to solve this problem
//
// Task2: Modify the spl_ls1.c program so that it omits the . and .. entries and
// sorts the entries by their times of last modification, with the more
// recent files preceding the less recent ones.
//
// Task3: write a version of ls that accepts
// one or more of the following options:
// -l # Display a listing for each file similar to the real ls.
// -F # Add one of the characters */=>@| to the end of the file
// to indicate its type.
// -g # Display each entry's group.

int main(int argc, char *argv[]) {
  DIR *dir = opendir(".");
  if (dir == NULL) {
    char *msg = "Open dir failed!";
    fatal_error(errno, msg);
  };

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    printf("%s\n", entry->d_name);
  };

  if (errno != 0) {
    char *msg = "Read dir failed!";
    fatal_error(errno, msg);
  };
}
