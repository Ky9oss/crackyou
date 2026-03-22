#include <locale.h>
#include <stdint.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>

#define DEBUG 0

#if DEBUG
#define debug(fmt, ...)                                                        \
  fprintf(stderr, "[DEBUG] %s:%d:%s(): " fmt "\n", __FILE__, __LINE__,         \
          __func__, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

// What exactly is __STDC_UTF_16__ ? A value in elf/exe section? A check in
// compilation? A check in systemcall?
// Answer: a compile-time thing
static_assert(__STDC_UTF_16__ == 1, "UTF-16 is not supported :-(");

static const uint32_t a = 0x12345678;

size_t utf8_to_utf16(size_t utf8_size, const char utf8[utf8_size],
                     char16_t *utf16) {
  size_t code, utf8_idx = 0, utf16_idx = 0;
  mbstate_t state = {0};
  while ((code = mbrtoc16(&utf16[utf16_idx], &utf8[utf8_idx],
                          utf8_size - utf8_idx, &state))) {
    switch (code) {
    case (size_t)-1: // invalid code unit sequence detected
      printf("%s\n", "invalid code unit sequence detected");
      return 0;
    case (size_t)-2: // code unit sequence missing elements
      printf("%s\n", "code unit sequence missing elements");
      return 0;
    case (size_t)-3: // high surrogate from a surrogate pair
      utf16_idx++;
      break;
    default: // one value written
      debug("The %dth code in utf8 have converted success\n", utf8_idx);
      utf16_idx++;
      utf8_idx += code;
    }
  }
  return utf16_idx + 1;
}

int main() {

  // `locale -a` in Linux to get all supported locale.
  if (!setlocale(LC_ALL, "en_HK.utf8")) {
    perror("setlocale failed");
    return 1;
  }
  // char utf8[] = u8"Confirmed. You will suffer. 尝试中文";
  char utf8[] = u8"尝试中文";

  size_t utf8_size = sizeof(utf8);
  printf("%s\n", utf8);
  printf("%zu UTF-8 code units: \n[", utf8_size);
  // for (size_t x = 0; x < utf8_size; ++x) {
  //   printf("%#x ", utf8[x]);
  // }
  unsigned char *bytes = (unsigned char *)utf8;
  for (size_t i = 0; i < utf8_size * sizeof(char); ++i) {
    printf("%#x ", bytes[i]);
  }
  puts("]");

  char16_t utf16[sizeof(utf8)]; // UTF-16 requires less code units than UTF-8
  size_t output_size = utf8_to_utf16(sizeof(utf8), utf8, utf16);
  printf("Converted to %zu UTF-16 code units: \n[", output_size);
  // for (size_t x = 0; x < output_size; ++x) {
  //   printf("%#x ", utf16[x]);
  // }
  bytes = (unsigned char *)utf16;
  for (size_t i = 0; i < output_size * sizeof(char16_t); ++i) {
    printf("%#x ", bytes[i]);
  }
  puts("]");
}
