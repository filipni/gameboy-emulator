#ifndef _PROC_H
#define _PROC_H

#include <stdint.h>

#define NUM_OPS 0x100

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
  
  char *mem;
} proc;

typedef int (*op) (proc*);

int run_operation(proc*, uint8_t); 

#endif

