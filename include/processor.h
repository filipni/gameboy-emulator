#ifndef _PROC_H
#define _PROC_H

#include <stdint.h>

#define NUM_OPS 0x100

typedef struct {
  uint8_t not_used:4;
  uint8_t z:1;
  uint8_t n:1;
  uint8_t h:1;
  uint8_t c:1;
} flags;

typedef union {
  struct
  {
    // little endian
    uint8_t low;
    uint8_t high;
  } r8;
  uint16_t r16;
} reg;

typedef struct {
  reg af;
  reg bc;
  reg de;
  reg hl;

  uint16_t sp;
  uint16_t pc;

  uint8_t interrupts_enabled;
} proc;

proc p;

typedef int (*op) ();

void init_proc();
void print_debug_info();
int run_operation();
int PREFIX();

#endif

