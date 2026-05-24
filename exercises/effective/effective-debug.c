#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#define name_size 10U

char *vstrcat(char *buff, size_t buff_length, ...) {
  char *ret = buff;
  va_list list;
  va_start(list, buff_length);

  const char *part = nullptr;
  size_t offset = 0;

  while ((part = va_arg(list, const char *))) {
    buff = (char *)memccpy(buff, part, '\0', buff_length - offset - 1);
    if (buff == nullptr) {
      ret[0] = '\0';
      break;
    }
    offset = buff - ret;
  }

  va_end(list);
  return ret;
}

int main() {
  char name[name_size] = "";
  char first[] = "Robert";
  char middle[] = "C.";
  char last[] = "Seacord";

  printf("%s\n", first);

  puts(vstrcat(name, sizeof(name), first, " ", middle, " ", last, nullptr));
}
