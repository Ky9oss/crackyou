#define _GNU_SOURCE
#include "common_hdrs.h"
#include "lastlog.h"
#include <paths.h>
#include <pwd.h>

#define BUFFER_SIZE 4096
#define FORMAT "%c"

int main() {
  char message[BUFFER_SIZE];
  char lastlog_time[64];
  struct lastlog ll;
  int origin_fd;
  int num_read;
  size_t ll_struct_size = sizeof(struct lastlog);
  time_t ll_time;
  struct tm *bdtime;
  struct passwd *passwd_entry;

  char *current_uname;
  uid_t current_uid;

  off_t offset;
  uid_t last_uid;

  if (setlocale(LC_ALL, "C.utf8") == NULL)
    fatal_error(LOCALE_ERROR, "setlocale() could not set the given locale");

  if ((origin_fd = open(_PATH_LASTLOG, O_RDONLY)) == -1) {
    sprintf(message, "unable to open %s for reading", _PATH_LASTLOG);
    fatal_error(errno, message);
  };

  offset = lseek(origin_fd, 0, SEEK_END);
  // printf("file size: %ld\n", offset);

  if ((offset % sizeof(struct lastlog)) == 0) {
    last_uid = offset / sizeof(struct lastlog);
    printf("lastlog structures: %d\n", last_uid);
  } else {
    error_mssge(-1, "lastlog size is wrong");
  };

  printf("%-16s %-8.8s %-16s %s\n", "Username", "Port", "From", "Last Login");

  setpwent();
  // struct passwd {
  //     char   *pw_name;       /* username */
  //     char   *pw_passwd;     /* user password */
  //     uid_t   pw_uid;        /* user ID */
  //     gid_t   pw_gid;        /* group ID */
  //     char   *pw_gecos;      /* user information */
  //     char   *pw_dir;        /* home directory */
  //     char   *pw_shell;      /* shell program */
  // };
  while (1) {
    if ((passwd_entry = getpwent()) != NULL) {
      current_uid = passwd_entry->pw_uid;
      current_uname = passwd_entry->pw_name;

      if (current_uid > last_uid) {
        continue;
      };

      if (lseek(origin_fd, current_uid * ll_struct_size, SEEK_SET) == -1)
        fatal_error(errno, "lseek");

      num_read = read(origin_fd, &ll, sizeof(struct lastlog));

      if (num_read <= 0) {
        if (0 != errno) /* A read error occurred. */
          fatal_error(errno, "read");
        else { /* Not a read error - shouldn't happen but continue */
          error_mssge(-1, "could not read the entry, skipping");
        }
      };

      if (ll.ll_time == 0) {
        printf("%-16s %-8.8s %-16s **Never logged in**\n", current_uname, " ",
               " ");
        continue;
      };

#if __WORDSIZE_TIME64_COMPAT32
      ll_time = ll.ll_time;
      bdtime = localtime(&ll_time);
#else
      bdtime = localtime(&(ll.ll_time));
#endif

      strftime(lastlog_time, sizeof(lastlog_time), FORMAT, bdtime);

      printf("%-16s %-8.8s %-16s %s\n", current_uname, ll.ll_line, ll.ll_host,
             lastlog_time);
      // printf("uid: %d\nname: %s\npassword: %s\nshell: %s\nhome_dir: %s\n\n",
      //        current_uid, passwd_entry->pw_name, passwd_entry->pw_passwd,
      //        passwd_entry->pw_shell, passwd_entry->pw_dir);
    } else {
      break;
    };
  };

  return 0;
}
