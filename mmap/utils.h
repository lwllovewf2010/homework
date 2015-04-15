#include <stdint.h>

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

int32_t CbPower(int32_t x, int32_t y);
int8_t CbAsciiToBin(int8_t value);
uint32_t CbStrLen(const uint8_t* buf);
uint32_t CbStrNCpy(uint8_t* dst, const uint8_t* src, uint32_t len);
uint64_t CbAsciiBufToBin(const uint8_t* buf);
uint32_t ReadMemMap(DbgrMemMap_t* pDbgrMemMapped, uint32_t max);
