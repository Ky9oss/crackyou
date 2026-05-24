#define _GNU_SOURCE /* See feature_test_macros(7) */
#include "common_hdrs.h"
#include <fcntl.h> /* Definition of AT_* constants */
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

//   File: error.c
//   Size: 558             Blocks: 8          IO Block: 4096   regular file
// Device: 8,1     Inode: 3941025     Links: 1
// Access: (0644/-rw-r--r--)  Uid: ( 1000/  user)   Gid: ( 1000/  user)
// Access: 2026-05-10 20:35:51.572903153 +0800
// Modify: 2026-05-10 20:35:49.256851278 +0800
// Change: 2026-05-10 20:35:49.260851368 +0800
//  Birth: 2026-05-10 20:35:49.256851278 +0800

void format_statx(struct statx statxbuf);
void time2str(char *time_str, size_t time_str_size,
              struct statx_timestamp *time_field);
void uid2name(int stx_uid, char *message);
void gid2group(int stx_gid, char *message);
char *mode2str(int stx_mode);
void dev2str(int stx_dev_major, int stx_dev_minor, char *str);

int main(int argc, char *argv[]) {
  char *path;
  int flags = 0;
  int opt;

  unsigned int mask = STATX_ATIME | STATX_MTIME | STATX_CTIME | STATX_BTIME |
                      STATX_UID | STATX_GID | STATX_BLOCKS | STATX_INO |
                      STATX_NLINK | STATX_TYPE;
  struct statx statxbuf;

  while ((opt = getopt(argc, argv, ":L")) != -1) {
    switch (opt) {
    case 'L':
      flags = AT_SYMLINK_NOFOLLOW;
      break;
    case '?':
      printf("Option is not supported");
      exit(EXIT_FAILURE);
    case ':':
      printf("Option has a missing argument");
      exit(EXIT_FAILURE);
    default: /* '?' */
      fprintf(stderr, "Usage: %s [-L] filepath\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind >= argc) {
    fprintf(stderr, "Expected argument after options\n");
    exit(EXIT_FAILURE);
  }

  path = argv[optind];

  printf("%6s: %s\n", "File", basename(argv[optind]));

  statx(AT_FDCWD, path, flags, mask, &statxbuf);

  format_statx(statxbuf);

  exit(EXIT_SUCCESS);
}

void format_statx(struct statx statxbuf) {
  char *mode_str = "";
  char uid_str[256];
  char dev_str[256];
  char gid_str[256];
  char mode[256];
  char access_time_str[256];
  char modify_time_str[256];
  char change_time_str[256];
  char birth_time_str[256];

  if (setlocale(LC_ALL, "") == NULL)
    fatal_error(LOCALE_ERROR, "setlocale() could not set the given locale");

  time2str(access_time_str, sizeof(access_time_str), &statxbuf.stx_atime);
  time2str(modify_time_str, sizeof(modify_time_str), &statxbuf.stx_mtime);
  time2str(birth_time_str, sizeof(birth_time_str), &statxbuf.stx_btime);
  time2str(change_time_str, sizeof(change_time_str), &statxbuf.stx_ctime);
  uid2name(statxbuf.stx_uid, uid_str);
  gid2group(statxbuf.stx_gid, gid_str);
  dev2str(statxbuf.stx_dev_major, statxbuf.stx_dev_minor, dev_str);
  mode_str = mode2str(statxbuf.stx_mode);
  sprintf(mode, "(%04o/%s)", statxbuf.stx_mode & 007777, mode_str);

  printf("%6s: %-20lld Blocks: %-20lld IO Block: %ld\n", "Size",
         (long long int)statxbuf.stx_size, (long long int)statxbuf.stx_blocks,
         (long int)statxbuf.stx_blksize);
  printf("%6s: %-20s Inode:  %-20lld Links:    %ld\n", "Device", dev_str,
         (long long int)statxbuf.stx_ino, (long int)statxbuf.stx_nlink);
  printf("%6s: %-20s Uid:    %-20s Gid:      %s\n", "Access", mode, uid_str,
         gid_str);
  printf("%6s: %s\n", "Access", access_time_str);
  printf("%6s: %s\n", "Modify", modify_time_str);
  printf("%6s: %s\n", "Change", change_time_str);
  printf("%6s: %s\n", "Birth", birth_time_str);
}

void time2str(char *time_str, size_t time_str_size,
              struct statx_timestamp *time_field) {

  time_t tt = time_field->tv_sec;
  struct tm *lt;
  lt = localtime(&tt);
  strftime(time_str, time_str_size, "%F %T", lt);
}

void uid2name(int stx_uid, char *message) {
  struct passwd *spwd;
  if ((spwd = getpwuid((uid_t)stx_uid)) == NULL) {
    fatal_error(errno, "get passwd from user_id failed");
  };
  sprintf(message, "( %d/  %s)", stx_uid, spwd->pw_name);
};

void gid2group(int stx_gid, char *message) {
  struct group *grent = getgrgid(stx_gid);
  sprintf(message, "( %d/  %s)", stx_gid, grent->gr_name);
}
void dev2str(int stx_dev_major, int stx_dev_minor, char *str) {
  sprintf(str, "%d,%d", stx_dev_major, stx_dev_minor);
}
char *mode2str(int stx_mode) {
  static char str[11];

  strcpy(str, "----------");
  if (S_ISDIR(stx_mode))
    str[0] = 'd';
  else if (S_ISREG(stx_mode))
    str[0] = '-';
  else if (S_ISCHR(stx_mode))
    str[0] = 'c';
  else if (S_ISBLK(stx_mode))
    str[0] = 'b';
  else if (S_ISLNK(stx_mode))
    str[0] = 'l';
  else if (S_ISFIFO(stx_mode))
    str[0] = 'p';
  else if (S_ISSOCK(stx_mode))
    str[0] = 's';

  if (stx_mode & S_IRUSR)
    str[1] = 'r';
  if (stx_mode & S_IWUSR)
    str[2] = 'w';
  if (stx_mode & S_IXUSR)
    str[3] = 'x';
  if (stx_mode & S_IRGRP)
    str[4] = 'r';
  if (stx_mode & S_IWGRP)
    str[5] = 'w';
  if (stx_mode & S_IXGRP)
    str[6] = 'x';
  if (stx_mode & S_IROTH)
    str[7] = 'r';
  if (stx_mode & S_IWOTH)
    str[8] = 'w';
  if (stx_mode & S_IXOTH)
    str[9] = 'x';
  /* Now check the setuid, setgid, and sticky bits. */
  if (stx_mode & S_ISUID)
    str[3] = 's';
  if (stx_mode & S_ISGID)
    str[6] = 's';
  if (stx_mode & S_ISVTX)
    str[9] = 't';

  return str;
}
