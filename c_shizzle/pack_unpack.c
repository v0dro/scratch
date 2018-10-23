#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef WORDS_BIGENDIAN
  #define IEEE_BIG_ENDIAN_P 1
  #define IEEE_LITTLE_ENDIAN_P NULL
#else
  #define IEEE_LITTLE_ENDIAN_P 1
  #define IEEE_BIG_ENDIAN_P NULL
#endif

typedef union ufloat {
  double f;
  unsigned u;
} ufloat;

void float32_pack(double x, unsigned char *p)
{
  int i;
  unsigned char a[4] = {0x00, 0x00,0x00,0x00};
  float y = (float)x;

  memcpy(a, &y, 4);
  
  for (i = 0; i < 4; ++i) {
    p[i] = a[i];
  }
}

void double64_pack(double x, unsigned char *p)
{
  int i;
  unsigned char a[8] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
  };

  memcpy(a, &x, 8);
  
  for (i = 0; i < 8; ++i) {
    p[i] = a[i];
  }
}

/* le : 1 is little endian. 0 is big-endian. */
double double64_unpack(unsigned char * p, int le)
{
  double num;

  /* if (le) { */
  /*   unsigned char buf[8]; */
  /*   char *d = &buf[7]; */
  /*   int i; */

  /*   for (i = 0; i < 8; ++i) { */
  /*     *d-- = *p++; */
  /*   } */
  /*  memcpy(&num, buf, 8); */
  /* } */

  memcpy(&num, p, 8);

  return num;
}

int main()
{
  /* pack */
  double a = 16448.0, num;
  unsigned char p[8];
  ufloat u;

  printf("big endian: %d.\n", IEEE_BIG_ENDIAN_P);
  printf("little endian: %d.\n", IEEE_LITTLE_ENDIAN_P);
  
  printf("A in hex: %#X.\n", a);

  float32_pack(a, p);
  printf("float32: %#X %#X %#X %#X.\n", p[0],
         p[1], p[2], p[3]);

  double64_pack(a, p);
  printf("double64: %#X %#X %#X %#X %#X %#X %#X %#X.\n", p[0],
         p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

  num = double64_unpack(p, 1);
  printf("unpacked double: %lf.", num);
  
  /* u.f  = 16448; */
  /* printf("binary: "); */
  /* for (int i = 0; i < 64; ++i) { */
  /*   if (u.u >> i & 0x1) putchar('1'); */
  /*   else putchar('0'); */
  /* } */
  
}
