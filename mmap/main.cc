#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#define PATH_MAPS   "/proc/self/maps"
#define LBUS_SIZE   512
#define MFILE_LEN   128
#define NR_SLOTS    100

typedef enum {
  PERM_RD = 0x01,
  PERM_WR = 0x02,
  PERM_EX = 0x04,
  PERM_PR = 0x08,
} DbgrMemPerm_t;

typedef struct {
  uint64_t start_addr;
  uint64_t end_addr;
  uint8_t  perm;
  uint8_t  filename[MFILE_LEN];
} DbgrMemMap_t;

int32_t CbPower(int32_t x, int32_t y) {
  int32_t sum = 0;
  if (!y) {
    return 1;
  } else {
    sum += x * CbPower(x, y - 1);
  }
  return sum;
}

int8_t CbAsciiToBin(int8_t value) {
  if ((value >= '0') && (value <= '9')) {
    return (value - '0');
  }
  if ((value >= 'A') && (value <= 'F')) {
    return (value - 'A') + 10;
  }
  if ((value >= 'a') && (value <= 'f')) {
    return (value - 'a') + 10;
  }
  return 0;
}

uint32_t CbStrLen(const uint8_t* buf) {
  uint32_t i;
  if (!buf) {
    return 0;
  }
  for (i = 0; buf[i]; ++i) {
  }
  return i;
}

uint32_t CbStrNCpy(uint8_t* dst, const uint8_t* src, uint32_t len) {
  uint32_t i;
  for (i = 0; i < len; ++i) {
    dst[i] = src[i];
  }
  dst[len] = '\0';
  return i;
}

uint64_t CbAsciiBufToBin(const uint8_t* buf) {
  uint64_t i, len, cal = 0;
  if (!buf) {
    return 0;
  }
  len = CbStrLen(buf);
  for (i = 0; i < len; i++) {
    cal += (CbAsciiToBin(buf[i]) * CbPower(16, len - i - 1));
  }
  return cal;
}

uint32_t ReadMemMap(DbgrMemMap_t* pDbgrMemMapped, uint32_t max) {
  int32_t fd;
  uint8_t lbuf[LBUS_SIZE];
  uint8_t* p = lbuf;
  int32_t rbyte;
  bool eof = false;
  bool eol = false;
  uint32_t idx = 0;
  uint32_t sidx = 0;
  uint32_t i = 0;

  // Clear memory
  memset(pDbgrMemMapped, 0, sizeof(DbgrMemMap_t) * max);

  // Open the file
  fd = open(PATH_MAPS, O_RDONLY);
  if (fd == -1) {
    return fd;
  }

  while (eof == false) {
    // Ensure it wouldn't overflow
    if (sidx >= max) {
      break;
    }

    // Read a byte to buffer
    rbyte = read(fd, p + idx, 1);
    if (rbyte <= 0) {
      // Reached the EOF
      eof = true;
      eol = true;
    }

    // If it's not at EOF, then check if it's at EOL
    if (eof == false) {
      if (*(p + idx) == '\n') {
        eol = true;
      }
    }

    // If it's not at EOL, then read next character
    if (eol == false) {
      idx++;
      continue;
    }

    // If no string left, then break the loop
    if (idx == 0) {
      break;
    }

    // Terminate the string
    *(p + idx) = '\0';

    // Process the line buffer
    // Read start address
    for (i = 0;; ++i) {
      if (*(p + i) == '-') {
        *(p + i) = '\0';
        pDbgrMemMapped[sidx].start_addr = CbAsciiBufToBin(p);
        break;
      }
    }
    // Read end address
    uint8_t* endp = p + i + 1;
    for (i = 0;; ++i) {
      if (*(endp + i) == ' ') {
        *(endp + i) = '\0';
        pDbgrMemMapped[sidx].end_addr = CbAsciiBufToBin(endp);
        break;
      }
    }
    // Read permission flags
    uint8_t* filep = endp + i + 1;
    if (*(filep + 0) == 'r') {
      pDbgrMemMapped[sidx].perm |= PERM_RD;
    }
    if (*(filep + 1) == 'w') {
      pDbgrMemMapped[sidx].perm |= PERM_WR;
    }
    if (*(filep + 2) == 'x') {
      pDbgrMemMapped[sidx].perm |= PERM_EX;
    }
    if (*(filep + 3) == 'p') {
      pDbgrMemMapped[sidx].perm |= PERM_PR;
    }
    // Read the mapped filename
    uint32_t cnt = 0;
    for (i = 0; *(filep + i) != '\0'; ++i) {
      if (*(filep + i) == ' ') {
        cnt++;
      }
      if (cnt <= 3) {
        continue;
      }
      break;
    }
    filep = filep + i;
    for (i = 0; *(filep + i) != '\0'; ++i) {
      if (*(filep + i) == ' ') {
        continue;
      }
      CbStrNCpy(pDbgrMemMapped[sidx].filename, filep + i, MFILE_LEN);
      break;
    }

    // If it's at EOF, then break the loop
    if (eof == true) {
      break;
    }

    // Reset the state
    eol = false;
    idx = 0;
    sidx++;
  }

  // Close the file and return
  close(fd);
  return sidx;
}

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
