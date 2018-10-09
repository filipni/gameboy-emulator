#ifndef _PROC_H
#define _PROC_H

#include <stdint.h>

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

void init_proc();
void print_debug_info();
int run_operation();

#endif

