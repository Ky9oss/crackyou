#include <stdio.h>
#include <assert.h>

int main() {
  FILE* tmp = tmpfile();
  fputs("Effective C\n", tmp);
  rewind(tmp);
  for (int c; (c = fgetc(tmp)) != EOF; putchar(c)) {}
  printf("%s", "End-of-file indicator ");
  puts(feof(tmp) ? "set" : "clear");
  printf("%s", "Error indicator ");
  puts(ferror(tmp) ? "set" : "clear");
  clearerr(tmp); // clear both indicators
  printf("%s", "End-of-file indicator ");
  puts(feof(tmp) ? "set" : "clear");
}
