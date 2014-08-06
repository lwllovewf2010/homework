#include <stdio.h>

int main( void ) {

  unsigned long long int a = 0x123456789ABCDEF;
  unsigned long int b = 0x987654321;

  a <<= 8;
  a |= 0xABULL;

  b >>= 8;
  b |= 0xAB000000ULL;

  printf( "A = 0x%8.8X%8.8X\n", (a >> 32) & 0xFFFFFFFFULL, a & 0xFFFFFFFULL );
  printf( "B = 0x%8.8X\n", b );

  return 0;
}

