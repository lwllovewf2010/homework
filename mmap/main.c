#include "utils.h"

#include <stdio.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

int main(int argc, char** argv) {
  DbgrMemMap_t bufDbgrMemMapped[NR_SLOTS];
  uint32_t cnt;
  uint32_t i;

  // Read memory map
  cnt = ReadMemMap(bufDbgrMemMapped, NR_SLOTS);

  // Dump data
  printf("Number of mapped file: %d\n", cnt);
  for (i = 0; i < cnt; ++i) {
    printf("start_addr = 0x%" PRIx64 "\n", bufDbgrMemMapped[i].start_addr);
    printf("end_addr   = 0x%" PRIx64 "\n", bufDbgrMemMapped[i].end_addr);
    printf("perm       = 0x%2.2X\n", bufDbgrMemMapped[i].perm);
    printf("file       = \"%s\"\n", bufDbgrMemMapped[i].filename);
    printf("========================================\n");
  }

  return 0;
}
