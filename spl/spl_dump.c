#define _GNU_SOURCE
#include "common_hdrs.h"
#include <paths.h>
#include <utmpx.h>

#define MESSAGE_SIZE 256
#define BUFFER_SIZE 4096

void dump_utmpx();

int main() {
  dump_utmpx();
  return 1;
}

typedef struct utmpx utmpx;

void dump_utmpx() {
  utmpx ut;
  char message[MESSAGE_SIZE];
  char *type;
  int fd;
  int result;

  if ((fd = open(_PATH_WTMP, O_RDONLY)) == -1) {
    sprintf(message, "unable to open %s for reading", _PATH_WTMP);
    fatal_error(errno, message);
  };

  printf("\n\n**WTMP**\n%10s %20s %10s %10s %30s\n", "Username", "Hostname", "Address",
         "Devicename", "Type");

  while (1) {
    result = read(fd, &ut, sizeof(utmpx));
    if (result == -1) {
      sprintf(message, "unable to read %s", _PATH_WTMP);
      fatal_error(errno, message);
    } else if (result == 0) {
      break;
    };
    switch (ut.ut_type) {

    case EMPTY: /* No valid user accounting information.  */
      type = "No valid user accounting information.";
      break;
    case RUN_LVL: /* The system's runlevel.  */
      type = "The system's runlevel.";
      break;
    case BOOT_TIME: /* Time of system boot.  */
      type = "Time of system boot.";
      break;
    case NEW_TIME: /* Time after system clock changed.  */
      type = "Time after system clock changed.";
      break;
    case OLD_TIME: /* Time when system clock changed.  */
      type = "Time when system clock changed.";
      break;
    case INIT_PROCESS: /* Process spawned by the init process.  */
      type = "Process spawned by the init process.";
      break;
    case LOGIN_PROCESS: /* Session leader of a logged in user.  */
      type = "Session leader of a logged in user.";
      break;
    case USER_PROCESS: /* Normal process.  */
      type = "Normal process.";
      break;
    case DEAD_PROCESS: /* Terminated process.  */
      type = "Terminated process.";
      break;
    case ACCOUNTING: /* System accounting.  */
      type = "System accounting.";
      break;
    default:
      break;
    };
    printf("%10s %20s %10d %10s %30s\n", ut.ut_user, ut.ut_host, *(ut.ut_addr_v6),
           ut.ut_line, type);
  };

  close(fd);

  if ((fd = open(_PATH_UTMP, O_RDONLY)) == -1) {
    sprintf(message, "unable to open %s for reading", _PATH_UTMP);
    fatal_error(errno, message);
  };

  printf("\n\n**UTMP**\n%10s %20s %10s %10s %30s\n", "Username", "Hostname", "Address",
         "Devicename", "Type");

  while (1) {
    result = read(fd, &ut, sizeof(utmpx));
    if (result == -1) {
      sprintf(message, "unable to read %s", _PATH_UTMP);
      fatal_error(errno, message);
    } else if (result == 0) {
      break;
    };
    switch (ut.ut_type) {

    case EMPTY: /* No valid user accounting information.  */
      type = "No valid user accounting information.";
      break;
    case RUN_LVL: /* The system's runlevel.  */
      type = "The system's runlevel.";
      break;
    case BOOT_TIME: /* Time of system boot.  */
      type = "Time of system boot.";
      break;
    case NEW_TIME: /* Time after system clock changed.  */
      type = "Time after system clock changed.";
      break;
    case OLD_TIME: /* Time when system clock changed.  */
      type = "Time when system clock changed.";
      break;
    case INIT_PROCESS: /* Process spawned by the init process.  */
      type = "Process spawned by the init process.";
      break;
    case LOGIN_PROCESS: /* Session leader of a logged in user.  */
      type = "Session leader of a logged in user.";
      break;
    case USER_PROCESS: /* Normal process.  */
      type = "Normal process.";
      break;
    case DEAD_PROCESS: /* Terminated process.  */
      type = "Terminated process.";
      break;
    case ACCOUNTING: /* System accounting.  */
      type = "System accounting.";
      break;
    default:
      break;
    };
    printf("%10s %20s %10d %10s %30s\n", ut.ut_user, ut.ut_host, *(ut.ut_addr_v6),
           ut.ut_line, type);
  };

  close(fd);
}
