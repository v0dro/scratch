#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUIRE_INT_OFFSET (31 + 1)
#define QUIRE_FRAC_OFFSET (31 + 1 + 240)
#define QUIRE_BYTE_LEN 64

#define QUIRE_INT_BYTE_OFFSET 8
#define QUIRE_FRAC_BYTE_OFFSET 34

typedef struct quire32 {
  unsigned char bytes[QUIRE_BYTE_LEN];
} quire32;

void
quire_print(quire32 *quire) {
  for (int i = 0; i < QUIRE_BYTE_LEN; ++i) {
    printf("%02X", (unsigned char)quire->bytes[i]);
  }
}

int main(int argc, char** argv) {
  quire32 quire;
  memset(quire.bytes, 0, QUIRE_BYTE_LEN);
  uint32_t a = 4;

  memcpy(quire.bytes + QUIRE_FRAC_BYTE_OFFSET - sizeof(uint32_t), &a, sizeof(uint32_t));

  quire_print(&quire);
}
