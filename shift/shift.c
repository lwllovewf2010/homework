#include <stdio.h>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

int main(int argc, char** argv) {
  int shift = 0;
  if (argc != 2) {
    return 0;
  }

  shift = atoi(argv[1]);
  printf("Shift %d\n", shift);
  printf("0x%16.16llX\n", (1ULL << shift));
  return 0;
}
