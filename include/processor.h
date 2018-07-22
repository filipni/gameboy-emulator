#include <stdint.h>

typedef union {
  struct 
  {
    uint8_t high;
    uint8_t low;
  } r8;
  uint16_t r16;
} reg;

typedef struct {
  reg AF;
  reg BC;
  reg DE;
  reg HL;
  uint16_t SP;
  uint16_t PC;
} processor;

int LD(uint8_t*, uint8_t);
