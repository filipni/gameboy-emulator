#include "processor.h"

int not_implemented(proc p) { return -1; }

int LD_imm(uint8_t* nn, uint8_t n)
{
  *nn = n; 
  return 8;
}

int LD_reg(uint8_t* r1, uint8_t* r2)
{
  *r1 = *r2;
  return 4;
}

int NOP(proc p) { return 4; }
