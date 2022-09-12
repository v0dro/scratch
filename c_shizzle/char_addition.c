#include <stdio.h>

int main() {
  unsigned char  a = 0xB0, b = 0x5C;
  unsigned char c = a + b;
  int cc = (int)a + (int)b;

  printf("c: %02X carrY: %d cc: %04X\n", c, c - 255, cc);

  unsigned char fc = 0xFF;
  unsigned char carry = 0x1;
  unsigned char add = fc + fc + 1;

  printf("sum: %02X\n", add);

  return 0;
}
