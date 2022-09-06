#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
/*
   NOTE: if you directly copy the uint32_t into the char array, you will face problems
   on a little endian machine, which the Mac Book Pro is. So direct use of memcpy for this
   purpose will lead to a wrong result.

   https://developer.ibm.com/articles/au-endianc/

   So to make sure that you are always using big-endian numbers you need to use the htonl() function.
 */

#define QUIRE_INT_OFFSET (31 + 1)
#define QUIRE_FRAC_OFFSET (31 + 1 + 240)
#define QUIRE_LEN 512
#define QUIRE_BYTE_LEN (QUIRE_LEN / CHAR_BIT)

#define QUIRE_INT_BYTE_OFFSET (QUIRE_INT_OFFSET / CHAR_BIT)
#define QUIRE_FRAC_BYTE_OFFSET (QUIRE_FRAC_OFFSET / CHAR_BIT)

typedef struct quire32 {
  unsigned char bytes[QUIRE_LEN / CHAR_BIT];
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
  uint32_t aa = 2;
  uint32_t a = htonl(aa);

  printf("uint32_t: %ld, offset: %ld sequence: %08X\n", sizeof(uint32_t),
         QUIRE_FRAC_BYTE_OFFSET - sizeof(uint32_t), a);

  memcpy(quire.bytes + 30, (char*)&a, 4);

  quire_print(&quire);
}
