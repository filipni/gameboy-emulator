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

  flags f;
  
  char *mem;
} proc;

typedef int (*op) (proc*);

int run_operation(proc*, uint8_t); 

int SUB(proc* p, uint8_t* r1, uint8_t r2, int carry);

#endif

