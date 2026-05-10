#define _GNU_SOURCE
#include "common_hdrs.h"
#include <paths.h>
#include <utmpx.h>

#define MESSAGE_SIZE 256
#define BUFFER_SIZE 4096
#define KERNEL_PAGE_SIZE 4096

// struct utmpx
// {
//   short int ut_type;		/* Type of login.  */
//   __pid_t ut_pid;		/* Process ID of login process.  */
//   char ut_line[__UT_LINESIZE]
//     __attribute_nonstring__;	/* Devicename.  */
//   char ut_id[4]
//     __attribute_nonstring__;	/* Inittab ID.  */
//   char ut_user[__UT_NAMESIZE]
//     __attribute_nonstring__;	/* Username.  */
//   char ut_host[__UT_HOSTSIZE]
//     __attribute_nonstring__;	/* Hostname for remote login.  */
//   struct __exit_status ut_exit;	/* Exit status of a process marked
// 				   as DEAD_PROCESS.  */
//
// /* The fields ut_session and ut_tv must be the same size when compiled
//    32- and 64-bit.  This allows files and shared memory to be shared
//    between 32- and 64-bit applications.  */
// #if __WORDSIZE_TIME64_COMPAT32
//   __int32_t ut_session;		/* Session ID, used for windowing.  */
//   struct
//   {
//     __uint32_t tv_sec;		/* Seconds.  */
//     __int32_t tv_usec;		/* Microseconds.  */
//   } ut_tv;			/* Time entry was made.  */
// #else
//   long int ut_session;		/* Session ID, used for windowing.  */
//   struct timeval ut_tv;		/* Time entry was made.  */
// #endif
//   __int32_t ut_addr_v6[4];	/* Internet address of remote host.  */
//   char __glibc_reserved[20];		/* Reserved for future use.  */
// };

// #define EMPTY		0	/* No valid user accounting information.
// */
//
// #ifdef __USE_GNU
// # define RUN_LVL	1	/* The system's runlevel.  */
// #endif
// #define BOOT_TIME	2	/* Time of system boot.  */
// #define NEW_TIME	3	/* Time after system clock changed.  */
// #define OLD_TIME	4	/* Time when system clock changed.  */
//
// #define INIT_PROCESS	5	/* Process spawned by the init process.  */
// #define LOGIN_PROCESS	6	/* Session leader of a logged in user.
// */ #define USER_PROCESS	7	/* Normal process.  */ #define
// DEAD_PROCESS	8	/* Terminated process.  */
//
// #ifdef __USE_GNU
// # define ACCOUNTING	9	/* System accounting.  */
// #endif

typedef struct utmp_list { /* Type of the linked list of utmpx records */
  struct utmpx ut;
  struct utmp_list *next;
  struct utmp_list *prev;
} utlist;

int get_prev_utrec(int fd, struct utmpx *ut, BOOL *finished);
void format_time_diff(time_t start_time, time_t end_time, char *time_diff_str);
void print_one_line(struct utmpx *ut, time_t end_time);

void save_ut_to_list(struct utmpx *ut, utlist **list);
void delete_utnode(utlist *p, utlist **list);
void erase_utlist(utlist **list);

int main() {
  int fd;

  if (setlocale(LC_ALL, "C.utf8") == NULL)
    fatal_error(LOCALE_ERROR, "setlocale() could not set the given locale");

  if ((fd = open(_PATH_WTMP, O_RDONLY)) == -1) {
    sprintf(message, "unable to open %s for reading", _PATH_WTMP);
    fatal_error(errno, message);
  };

  offset = lseek(fd, 0, SEEK_END);
  // TODO: This sick program has not done. I'm just tired.

  printf("%ld\n", sizeof(struct utmpx));
  return 0;
}

void save_ut_to_list(struct utmpx *ut, utlist **list) {
  utlist *utmp_node_ptr;
  if (NULL == (utmp_node_ptr = (utlist *)malloc(sizeof(utlist))))
    fatal_error(errno, "malloc");
  memcpy(&(utmp_node_ptr->ut), ut, sizeof(struct utmpx));
  utmp_node_ptr->next = *list;
  utmp_node_ptr->prev = NULL;
  if (NULL != *list)
    (*list)->prev = utmp_node_ptr;
  *list = utmp_node_ptr;
}

void delete_utnode(utlist *p, utlist **list) {
  if (p->next != NULL) {
    p->next->prev = p->prev;
  };

  if (p->prev != NULL) {
    p->prev->next = p->next;
  };

  free(p);
}

void erase_utlist(utlist **list) {
  utlist *ptr;

  while (TRUE) {
    if ((*list)->next != NULL) {
      ptr = *list;
      *list = (*list)->next;
      free(ptr);
    };
  };
}

void format_time_diff(time_t start_time, time_t end_time, char *time_diff_str) {
  time_t duration = start_time - end_time;
  time_t minute = (duration / 60) % 60;
  time_t hour = (duration / 3600) % 3600;
  time_t day = (duration / (24 * 3600)) % 24 * 3600;

  if (day > 0) {
    sprintf(time_diff_str, "%ld+%2ld:%2ld", day, hour, minute);
  } else {
    sprintf(time_diff_str, "%2ld:%2ld", hour, minute);
  };
}

void print_one_line(struct utmpx *ut, time_t end_time) {
  char *duration;
  struct tm *tm_login;
  struct tm *tm_logout;
  char *start_date_fmt = "%a %b %d %H:%M";
  char *end_date_fmt = "%H:%M";
  char formatted_login[200];
  char formatted_logout[200];

  if (end_time == 0) {
    if (ut->ut_type == BOOT_TIME) {
      duration = "still running";
    } else if (ut->ut_type == USER_PROCESS) {
      duration = "still logged in";
    }
  } else if (end_time == -1) {
    duration = "gonne - no logout";
  } else {
    format_time_diff(ut->ut_tv.tv_sec, end_time, duration);
  };

  time_t time = ut->ut_tv.tv_sec;

  if ((tm_login = localtime(&time)) == NULL) {
    fatal_error(errno, "login timep to tm error");
  };

  if (strftime(formatted_login, sizeof(formatted_login), start_date_fmt,
               tm_login) == 0) {
    fatal_error(errno, "strftime error");
  };

  if ((tm_logout = localtime(&(end_time))) == NULL) {
    fatal_error(errno, "login timep to tm error");
  };

  if (strftime(formatted_logout, sizeof(formatted_logout), end_date_fmt,
               tm_logout) == 0) {
    fatal_error(errno, "strftime error");
  };

  printf("%-8.8s %-12.12s %-18s %s - %s %s\n", ut->ut_user, ut->ut_line,
         ut->ut_host, formatted_login, formatted_logout, duration);
}
