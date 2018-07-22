#ifndef _PROC_H
#define _PROC_H

#include <stdint.h>

#define NUM_OPS 0x100

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
} proc;

typedef int (*op) (proc);

int LD_imm(uint8_t*, uint8_t);
int LD_reg(uint8_t*, uint8_t*);

int NOP(proc p);
int not_implemented(proc p);

#endif

