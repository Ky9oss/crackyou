// Some examples for SYSTEM PROGRAMMING IN LINUX
//
// Author: Ky9oss

#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

extern char **environ;

int spl_environ(){
    char **envp = environ;
    while ( NULL != *envp ) {
        printf("%s\n", *envp);
        envp++;
    }
    return 0;
}

int spl_gethostname() {
  char name[4];
  size_t len = 3;
  int returnvalue;

  returnvalue = gethostname(name, len);
  if (-1 == returnvalue) {
    switch (errno) {
    case EFAULT:
      printf("EFAULT: name is an invalid address\n");
      break;
    case EINVAL:
      printf("EINVAL: len is negative\n");
      break;
    case ENAMETOOLONG:
      printf("ENAMETOOLONG: len is smaller than the actual size\n");
      break;
    }
  }

  return 0;
}

int spl_perror() {
  char name[4];   /* Declare string to hold returned value. */
  size_t len = 3; /* Purposely declared too small so error is revealed */
  int returnvalue;
  returnvalue = gethostname(name, len); /* Make the call. */
  if (-1 == returnvalue) {
    perror("gethostname");
    // printf("FLAG1\n");
  }
  else
    printf("%s\n", name);

  return 0;
}

int spl_gettid_syscall() {
  // same
  printf("syscall(SYS_gettid): %ld\n", syscall(SYS_gettid));
  printf("syscall(__NR_gettid): %ld\n", syscall(__NR_gettid));
  // printf("gettid(): I'm the process with thread-id: %d\n", gettid());
  return 0;
}

int spl_getenv() {

  char *shell = getenv("SHELL");
  printf("The current shell is %s\n", shell);
  return 0;
}

int spl_getpid() {
  // System call
  printf("I'm the process with process-id: %d\n", getpid());
  return 0;
}

int main() {
  spl_environ();
  return 0;
}
