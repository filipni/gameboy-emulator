#include <stdio.h>
#include <stdint.h>
#include "constants.h"

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
  reg SP;
  reg PC;
} cpu;

int main(int argc, char* argv[])
{
  printf("Hello world!\n");
  return 0;
}
