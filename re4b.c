#include <stdio.h>

char *amsg(int n, char *s) {
  char buf[100];
  sprintf(buf, "error %d: %s\n", n, s);
  return buf;
};
char *interim(int n, char *s) {
  char large_buf[8000];
  // make use of local array.
  // it will be optimized away otherwise, as useless.
  large_buf[0] = 0;
  return amsg(n, s);
};

int main() {
  // This will succeed in MSVC but wrong in GCC. Because GCC optimize the return
  //    value in amsg to 0
  /* amsg:
          push	rbp
          mov	rbp, rsp
          add	rsp, -128
          mov	DWORD PTR -116[rbp], edi
          mov	QWORD PTR -128[rbp], rsi
          mov	rcx, QWORD PTR -128[rbp]
          mov	edx, DWORD PTR -116[rbp]
          lea	rsi, .LC0[rip]
          lea	rax, -112[rbp]
          mov	rdi, rax
          mov	eax, 0
          call	sprintf@PLT
          mov	eax, 0
          leave
          ret
          */
  printf("%s\n", interim(1234, "something wrong!"));
  return 1;
}
