#define _GNU_SOURCE
#include "common_hdrs.h"

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif

#define MESSAGE_SIZE 256
#define PERMISSIONS S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH /*rw-rw-r--*/

int main(int argc, char *argv[]) {

  char message[MESSAGE_SIZE];
  char buffer[BUFFER_SIZE];
  char *pathname = "temp_origin.txt";
  char *target_pathname = "temp_target.txt";
  int origin_fd;
  int target_fd;
  int num_read;
  int num_write;

  errno = 0;
  if ((origin_fd = open(pathname, O_RDONLY)) == -1) {
    sprintf(message, "unable to open %s for reading", pathname);
    fatal_error(errno, message);
  };
  if ((target_fd = open(target_pathname, O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS)) == -1) {
    sprintf(message, "unable to open %s for writing", target_pathname);
    fatal_error(errno, message);
  };

  while (TRUE) {
    num_read = read(origin_fd, buffer, BUFFER_SIZE);
    if (num_read == -1) {
      sprintf(message, "unable to read %s", pathname);
      fatal_error(errno, message);
    };
    if ((num_write = write(target_fd, buffer, num_read)) == -1){
      sprintf(message, "unable to write");
      fatal_error(errno, message);
    };
    if (num_write != num_read) {
      sprintf(message, "incomplete writing");
      fatal_error(errno, message);
    };
    if (num_read < BUFFER_SIZE) {
        break;
    };
  };

  close(origin_fd);
  close(target_fd);

  printf("Done: %s copied to %s.", pathname, target_pathname);
  return 0;
}
