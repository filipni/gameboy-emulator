#include "processor.h"
#include "utils.h"
#include <string.h>

int not_implemented(proc* p) { return -1; }

uint8_t calculate_half_carry(int v1, int v2)
{
  int nibble = v1 & 0x0F;
  nibble += v2 & 0x0F;

  if (nibble >= 0x10)
    return 1;
  else
    return 0;
}

uint8_t calculate_carry(int v1, int v2)
{
  int byte = v1 & 0xFF;
  byte += v2 & 0xFF;

  if (byte >= 0x100)
    return 1;
  else
    return 0;
}

int INC_r8(proc* p, uint8_t* r)
{
  p->f.h = calculate_half_carry(*r, 1);
  
  (*r)++;

  if (*r == 0)
    p->f.z = 1;
  else
    p->f.z = 0;
  p->f.n = 0;

  p->pc++;
  return 4;
}

int INC_B(proc* p) { return INC_r8(p, &p->bc.r8.high); }
int INC_D(proc* p) { return INC_r8(p, &p->de.r8.high); }
int INC_H(proc* p) { return INC_r8(p, &p->hl.r8.high); }
int INC_mHL(proc* p) { return INC_r8(p, &p->mem[p->hl.r16]); }

int INC_C(proc* p) { return INC_r8(p, &p->bc.r8.low); }
int INC_E(proc* p) { return INC_r8(p, &p->de.r8.low); }
int INC_L(proc* p) { return INC_r8(p, &p->hl.r8.low); }
int INC_A(proc* p) { return INC_r8(p, &p->af.r8.high); }

int DEC_r8(proc* p, uint8_t* r)
{
  p->f.h = calculate_half_carry(*r, -1);
  
  (*r)--;

  if (*r == 0)
    p->f.z = 1;
  else
    p->f.z = 0;
  p->f.n = 1;

  p->pc++;
  return 4;
}

int DEC_B(proc* p) { return DEC_r8(p, &p->bc.r8.high); }
int DEC_D(proc* p) { return DEC_r8(p, &p->de.r8.high); }
int DEC_H(proc* p) { return DEC_r8(p, &p->hl.r8.high); }
int DEC_mHL(proc* p) { return DEC_r8(p, &p->mem[p->hl.r16]); }

int DEC_C(proc* p) { return DEC_r8(p, &p->bc.r8.low); }
int DEC_E(proc* p) { return DEC_r8(p, &p->de.r8.low); }
int DEC_L(proc* p) { return DEC_r8(p, &p->hl.r8.low); }
int DEC_A(proc* p) { return DEC_r8(p, &p->af.r8.high); }

int LD_mHL_A_inc(proc* p) 
{
  p->mem[p->hl.r16++] = p->af.r8.high;

  p->pc++;
  return 8;
}

int LD_mHL_A_dec(proc* p) 
{
  p->mem[p->hl.r16--] = p->af.r8.high;

  p->pc++;
  return 8;
}

int LD_A_mHL_inc(proc* p) 
{
  p->af.r8.high = p->mem[p->hl.r16++];  

  p->pc++;
  return 8;
}

int LD_A_mHL_dec(proc* p)
{
  p->af.r8.high = p->mem[p->hl.r16--];

  p->pc++;
  return 8;
}

int INC_r16(proc* p, uint16_t* r)
{
  *r++;

  p->pc++;
  return 8;
}

int INC_BC(proc* p) { return INC_r16(p, &p->bc.r16); }
int INC_DE(proc* p) { return INC_r16(p, &p->de.r16); }
int INC_HL(proc* p) { return INC_r16(p, &p->hl.r16); }
int INC_SP(proc* p) { return INC_r16(p, &p->sp); }

int DEC(proc* p, uint16_t* r)
{
  *r--;

  p->pc++;
  return 8;
}

int DEC_BC(proc* p) { return DEC(p, &p->bc.r16); }
int DEC_DE(proc* p) { return DEC(p, &p->de.r16); }
int DEC_HL(proc* p) { return DEC(p, &p->hl.r16); }
int DEC_SP(proc* p) { return DEC(p, &p->sp); }

int LD_d8(proc* p, uint8_t* nn)
{
  *nn = p->mem[p->pc+1];
 
  p->pc += 2;
  return 8;
}

int LD_B(proc* p) { LD_d8(p, &p->bc.r8.high); }
int LD_D(proc* p) { LD_d8(p, &p->de.r8.high); }
int LD_H(proc* p) { LD_d8(p, &p->hl.r8.high); }
int LD_mHL(proc* p) { LD_d8(p, (uint8_t*) (p->mem + p->hl.r16)); }

int LD_C(proc* p) { LD_d8(p, &p->bc.r8.low); }
int LD_E(proc* p) { LD_d8(p, &p->de.r8.low); }
int LD_L(proc* p) { LD_d8(p, &p->hl.r8.low); }
int LD_A(proc* p) { LD_d8(p, &p->af.r8.high); }

int LD_d16(proc* p, reg* r)
{
  r->r8.low = p->mem[p->pc+1]; 
  r->r8.high = p->mem[p->pc+2]; 

  p->pc += 3;
  return 12;
}

int LD_BC(proc* p) { return LD_d16(p, &p->bc); }
int LD_DE(proc* p) { return LD_d16(p, &p->de); }
int LD_HL(proc* p) { return LD_d16(p, &p->hl); }
int LD_SP(proc* p) 
{ 
  uint8_t low = p->mem[p->pc+1]; 
  uint8_t high = p->mem[p->pc+2]; 
  
  p->sp = generate_address(low, high);

  p->pc += 3;
  return 12;
}

int LD_reg_mem(proc* p, uint8_t* reg, uint8_t* mem) 
{ 
  *reg = *mem;

  p->pc++;
  return 8;
}

int LD_mBC_A(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->bc.r16);
  return LD_reg_mem(p, mem_pos, &p->af.r8.high); 
}
int LD_mDE_A(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->af.r8.high); 
}
int LD_A_mBC(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->bc.r16);
  return LD_reg_mem(p, &p->af.r8.high, mem_pos); 
}
int LD_A_mDE(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, &p->af.r8.high, mem_pos); 
}
int LD_B_mHL(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->hl.r16);
  return LD_reg_mem(p, &p->bc.r8.high, mem_pos); 
}
int LD_D_mHL(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->hl.r16);
  return LD_reg_mem(p, &p->de.r8.high, mem_pos); 
}
int LD_H_mHL(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->hl.r16);
  return LD_reg_mem(p, &p->hl.r8.high, mem_pos); 
}
int LD_C_mHL(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->hl.r16);
  return LD_reg_mem(p, &p->bc.r8.low, mem_pos); 
}
int LD_E_mHL(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->hl.r16);
  return LD_reg_mem(p, &p->de.r8.low, mem_pos); 
}
int LD_L_mHL(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->hl.r16);
  return LD_reg_mem(p, &p->hl.r8.low, mem_pos); 
}
int LD_A_mHL(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->hl.r16);
  return LD_reg_mem(p, &p->af.r8.low, mem_pos); 
}
int LD_mHL_B(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->bc.r8.high); 
}
int LD_mHL_C(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->bc.r8.low); 
}
int LD_mHL_D(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->de.r8.high); 
}
int LD_mHL_E(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->de.r8.low); 
}
int LD_mHL_H(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->hl.r8.high); 
}
int LD_mHL_L(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->hl.r8.low); 
}
int LD_mHL_A(proc* p)
{
  uint8_t* mem_pos = (uint8_t*) (p->mem + p->de.r16);
  return LD_reg_mem(p, mem_pos, &p->af.r8.high); 
}

int LD_reg(proc* p, uint8_t* r1, uint8_t* r2)
{
  *r1 = *r2;

  p->pc++;
  return 4;
}

int LD_B_B(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->bc.r8.high); }
int LD_B_C(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->bc.r8.low); }
int LD_B_D(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->de.r8.high); }
int LD_B_E(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->de.r8.low); }
int LD_B_H(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->hl.r8.high); }
int LD_B_L(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->hl.r8.low); }
int LD_B_A(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->af.r8.high); }

int LD_C_B(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->bc.r8.high); }
int LD_C_C(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->bc.r8.low); }
int LD_C_D(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->de.r8.high); }
int LD_C_E(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->de.r8.low); }
int LD_C_H(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->hl.r8.high); }
int LD_C_L(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->hl.r8.low); }
int LD_C_A(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->af.r8.high); }

int LD_D_B(proc* p) { return LD_reg(p, &p->de.r8.high, &p->bc.r8.high); }
int LD_D_C(proc* p) { return LD_reg(p, &p->de.r8.high, &p->bc.r8.low); }
int LD_D_D(proc* p) { return LD_reg(p, &p->de.r8.high, &p->de.r8.high); }
int LD_D_E(proc* p) { return LD_reg(p, &p->de.r8.high, &p->de.r8.low); }
int LD_D_H(proc* p) { return LD_reg(p, &p->de.r8.high, &p->hl.r8.high); }
int LD_D_L(proc* p) { return LD_reg(p, &p->de.r8.high, &p->hl.r8.low); }
int LD_D_A(proc* p) { return LD_reg(p, &p->de.r8.high, &p->af.r8.high); }

int LD_E_B(proc* p) { return LD_reg(p, &p->de.r8.low, &p->bc.r8.high); }
int LD_E_C(proc* p) { return LD_reg(p, &p->de.r8.low, &p->bc.r8.low); }
int LD_E_D(proc* p) { return LD_reg(p, &p->de.r8.low, &p->de.r8.high); }
int LD_E_E(proc* p) { return LD_reg(p, &p->de.r8.low, &p->de.r8.low); }
int LD_E_H(proc* p) { return LD_reg(p, &p->de.r8.low, &p->hl.r8.high); }
int LD_E_L(proc* p) { return LD_reg(p, &p->de.r8.low, &p->hl.r8.low); }
int LD_E_A(proc* p) { return LD_reg(p, &p->de.r8.low, &p->af.r8.high); }

int LD_H_B(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->bc.r8.high); }
int LD_H_C(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->bc.r8.low); }
int LD_H_D(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->de.r8.high); }
int LD_H_E(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->de.r8.low); }
int LD_H_H(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->hl.r8.high); }
int LD_H_L(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->hl.r8.low); }
int LD_H_A(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->af.r8.high); }

int LD_L_B(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->bc.r8.high); }
int LD_L_C(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->bc.r8.low); }
int LD_L_D(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->de.r8.high); }
int LD_L_E(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->de.r8.low); }
int LD_L_H(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->hl.r8.high); }
int LD_L_L(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->hl.r8.low); }
int LD_L_A(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->af.r8.high); }

int LD_A_B(proc* p) { return LD_reg(p, &p->af.r8.high, &p->bc.r8.high); }
int LD_A_C(proc* p) { return LD_reg(p, &p->af.r8.high, &p->bc.r8.low); }
int LD_A_D(proc* p) { return LD_reg(p, &p->af.r8.high, &p->de.r8.high); }
int LD_A_E(proc* p) { return LD_reg(p, &p->af.r8.high, &p->de.r8.low); }
int LD_A_H(proc* p) { return LD_reg(p, &p->af.r8.high, &p->hl.r8.high); }
int LD_A_L(proc* p) { return LD_reg(p, &p->af.r8.high, &p->hl.r8.low); }
int LD_A_A(proc* p) { return LD_reg(p, &p->af.r8.high, &p->af.r8.high); }

int NOP(proc* p) { p->pc++; return 4; }

int JP(proc* p)
{
  uint16_t adr = generate_address(p->mem[p->pc+1], p->mem[p->pc+2]);
  p->pc = adr;
  return 16;
}

int COND_JP(proc* p, uint8_t cond)
{
  if (cond)
  {
    uint16_t adr = generate_address(p->mem[p->pc+1], p->mem[p->pc+2]);
    p->pc = adr;
    return 16;
  }

  p->pc += 3;
  return 12; 
}

int JP_NZ(proc* p) { return COND_JP(p, !p->f.z); }
int JP_Z(proc* p) { return COND_JP(p, p->f.z); }
int JP_NC(proc* p) { return COND_JP(p, !p->f.c); }
int JP_C(proc* p) { return COND_JP(p, p->f.c); }

int XOR(proc* p, uint8_t* reg, uint8_t arg)
{
  *reg |= arg;

  // set flags
  memset(&p->f, 0, sizeof(flags));
  if (*reg == 0)
    p->f.z = 1;
  
  p->pc++;
  return 4;
}

int XOR_B(proc* p) { return XOR(p, &p->bc.r8.high, p->mem[p->pc+1]); }
int XOR_C(proc* p) { return XOR(p, &p->bc.r8.low, p->mem[p->pc+1]); }
int XOR_D(proc* p) { return XOR(p, &p->de.r8.high, p->mem[p->pc+1]); }
int XOR_E(proc* p) { return XOR(p, &p->de.r8.low, p->mem[p->pc+1]); }
int XOR_H(proc* p) { return XOR(p, &p->hl.r8.high, p->mem[p->pc+1]); }
int XOR_L(proc* p) { return XOR(p, &p->hl.r8.low, p->mem[p->pc+1]); }
int XOR_A(proc* p) { return XOR(p, &p->af.r8.high, p->mem[p->pc+1]); }

int POP(proc* p, uint16_t* r)
{
  uint16_t val = generate_address(p->mem[p->sp], p->mem[p->sp+1]);
  *r = val;
  
  p->sp += 2;
  p->pc += 1;
  return 12;
}

int POP_BC(proc* p) { return POP(p, &p->bc.r16); }
int POP_DE(proc* p) { return POP(p, &p->de.r16); }
int POP_HL(proc* p) { return POP(p, &p->hl.r16); }
int POP_AF(proc*p)  { return POP(p, &p->af.r16); }

int PUSH(proc* p, reg* r)
{
  p->mem[p->sp+1] = r->r8.high;
  p->mem[p->sp+2] = r->r8.low;

  p->sp += 2;
  p->pc += 1;
  return 16;
}

int PUSH_BC(proc* p) { return PUSH(p, &p->bc); }
int PUSH_DE(proc* p) { return PUSH(p, &p->de); }
int PUSH_HL(proc* p) { return PUSH(p, &p->hl); }
int PUSH_AF(proc* p) { return PUSH(p, &p->af); }

int RST(proc* p, uint8_t addr)
{
  p->mem[p->sp+1] = p->pc;
  p->pc = addr;

  p->sp++;
  return 16;
}

int RST_00(proc* p) { return RST(p, 0x00); }
int RST_10(proc* p) { return RST(p, 0x10); }
int RST_20(proc* p) { return RST(p, 0x20); }
int RST_30(proc* p) { return RST(p, 0x30); }

int RST_08(proc* p) { return RST(p, 0x08); }
int RST_18(proc* p) { return RST(p, 0x18); }
int RST_28(proc* p) { return RST(p, 0x28); }
int RST_38(proc* p) { return RST(p, 0x38); }

int ADD(proc* p, uint8_t* r1, uint8_t r2, int carry)
{
  int c = carry;

  int res;
  for (int i = 0; i < 8; i++)
  {
    uint8_t bitmask = 1 << i; 
    int res = ((*r1) & bitmask) + (r2 & bitmask) + c;
    c = res > bitmask;
    if (i == 3) { p->f.h = c; }
  }

  p->f.c = c;
  p->f.n = 0;
  p->f.z = !(*r1);

  *r1 += r2;

  p->pc++;
  return 4;
}

int ADD_A_B(proc* p) { return ADD(p, &p->af.r8.high, p->bc.r8.high, 0); } 
int ADD_A_C(proc* p) { return ADD(p, &p->af.r8.high, p->bc.r8.low, 0); } 
int ADD_A_D(proc* p) { return ADD(p, &p->af.r8.high, p->de.r8.high, 0); } 
int ADD_A_E(proc* p) { return ADD(p, &p->af.r8.high, p->de.r8.low, 0); } 
int ADD_A_H(proc* p) { return ADD(p, &p->af.r8.high, p->hl.r8.high, 0); } 
int ADD_A_L(proc* p) { return ADD(p, &p->af.r8.high, p->hl.r8.low, 0); } 
int ADD_A_mHL(proc* p) { return ADD(p, &p->af.r8.high, p->mem[p->hl.r16], 0); }
int ADD_A_A(proc* p) { return ADD(p, &p->af.r8.high, p->af.r8.high, 0); }

int ADC_A_B(proc* p) { return ADD(p, &p->af.r8.high, p->bc.r8.high, p->f.c); } 
int ADC_A_C(proc* p) { return ADD(p, &p->af.r8.high, p->bc.r8.low, p->f.c); } 
int ADC_A_D(proc* p) { return ADD(p, &p->af.r8.high, p->de.r8.high, p->f.c); } 
int ADC_A_E(proc* p) { return ADD(p, &p->af.r8.high, p->de.r8.low, p->f.c); } 
int ADC_A_H(proc* p) { return ADD(p, &p->af.r8.high, p->hl.r8.high, p->f.c); } 
int ADC_A_L(proc* p) { return ADD(p, &p->af.r8.high, p->hl.r8.low, p->f.c); } 
int ADC_A_mHL(proc* p) { return ADD(p, &p->af.r8.high, p->mem[p->hl.r16], p->f.c); }
int ADC_A_A(proc* p) { return ADD(p, &p->af.r8.high, p->af.r8.high, p->f.c); }

op operations[NUM_OPS] = { 
  NOP,              // 0x00
  LD_BC,            // 0x01
  LD_mBC_A,         // 0x02
  INC_BC,           // 0x03
  INC_B,            // 0x04
  DEC_B,            // 0x05
  LD_B,             // 0x06
  not_implemented,  // 0x07
  DEC_BC,           // 0x08
  not_implemented,  // 0x09
  LD_A_mBC,         // 0x0a
  not_implemented,  // 0x0b
  INC_C,            // 0x0c
  DEC_C,            // 0x0d
  LD_C,             // 0x0e
  not_implemented,  // 0x0f
  not_implemented,  // 0x10
  LD_DE,            // 0x11
  LD_mDE_A,         // 0x12
  INC_DE,           // 0x13
  INC_D,            // 0x14
  DEC_D,            // 0x15
  LD_D,             // 0x16
  not_implemented,  // 0x17
  DEC_DE,           // 0x18
  not_implemented,  // 0x19
  LD_A_mDE,         // 0x1a
  not_implemented,  // 0x1b
  INC_E,            // 0x1c
  DEC_E,            // 0x1d
  LD_E,             // 0x1e
  not_implemented,  // 0x1f
  not_implemented,  // 0x20
  LD_HL,            // 0x21
  LD_mHL_A_inc,     // 0x22
  INC_HL,           // 0x23
  INC_H,            // 0x24
  DEC_H,            // 0x25
  LD_H,             // 0x26
  not_implemented,  // 0x27
  DEC_HL,           // 0x28
  not_implemented,  // 0x29
  LD_A_mHL_inc,     // 0x2a
  not_implemented,  // 0x2b
  INC_L,            // 0x2c
  DEC_L,            // 0x2d
  LD_L,             // 0x2e
  not_implemented,  // 0x2f
  not_implemented,  // 0x30
  LD_SP,            // 0x31
  LD_mHL_A_dec,     // 0x32
  INC_SP,           // 0x33
  INC_mHL,          // 0x34
  DEC_mHL,          // 0x35
  LD_mHL,           // 0x36
  not_implemented,  // 0x37
  DEC_SP,           // 0x38
  not_implemented,  // 0x39
  LD_A_mHL_dec,     // 0x3a
  not_implemented,  // 0x3b
  INC_A,            // 0x3c
  DEC_A,            // 0x3d
  LD_A,             // 0x3e
  not_implemented,  // 0x3f
  LD_B_B,           // 0x40
  LD_B_C,           // 0x41
  LD_B_D,           // 0x42
  LD_B_E,           // 0x43
  LD_B_H,           // 0x44
  LD_B_L,           // 0x45
  LD_B_mHL,         // 0x46
  LD_B_A,           // 0x47
  LD_C_B,           // 0x48
  LD_C_C,           // 0x49
  LD_C_D,           // 0x4a
  LD_C_E,           // 0x4b
  LD_C_H,           // 0x4c
  LD_C_L,           // 0x4d
  LD_C_mHL,         // 0x4e
  LD_C_A,           // 0x4f
  LD_D_B,           // 0x50
  LD_D_C,           // 0x51
  LD_D_D,           // 0x52
  LD_D_E,           // 0x53
  LD_D_H,           // 0x54
  LD_D_L,           // 0x55
  LD_D_mHL,         // 0x56
  LD_D_A,           // 0x57 
  LD_E_B,           // 0x58
  LD_E_C,           // 0x59
  LD_E_D,           // 0x5a
  LD_E_E,           // 0x5b
  LD_E_H,           // 0x5c
  LD_E_L,           // 0x5d
  LD_E_mHL,         // 0x5e
  LD_E_A,           // 0x5f 
  LD_H_B,           // 0x60
  LD_H_C,           // 0x61
  LD_H_D,           // 0x62
  LD_H_E,           // 0x63
  LD_H_H,           // 0x64
  LD_H_L,           // 0x65
  LD_H_mHL,         // 0x66
  LD_H_A,           // 0x67 
  LD_L_B,           // 0x68
  LD_L_C,           // 0x69
  LD_L_D,           // 0x6a
  LD_L_E,           // 0x6b
  LD_L_H,           // 0x6c
  LD_L_L,           // 0x6d
  LD_L_mHL,         // 0x6e
  LD_L_A,           // 0x6f 
  LD_mHL_B,         // 0x70
  LD_mHL_C,         // 0x71
  LD_mHL_D,         // 0x72
  LD_mHL_E,         // 0x73
  LD_mHL_H,         // 0x74
  LD_mHL_L,         // 0x75
  not_implemented,  // 0x76
  LD_mHL_A,         // 0x77
  LD_A_B,           // 0x78
  LD_A_C,           // 0x79
  LD_A_D,           // 0x7a
  LD_A_E,           // 0x7b
  LD_A_H,           // 0x7c
  LD_A_L,           // 0x7d
  LD_A_mHL,         // 0x7e
  LD_A_A,           // 0x7f
  not_implemented,  // 0x80
  not_implemented,  // 0x81
  not_implemented,  // 0x82
  not_implemented,  // 0x83
  not_implemented,  // 0x84
  not_implemented,  // 0x85
  not_implemented,  // 0x86
  not_implemented,  // 0x87
  not_implemented,  // 0x88
  not_implemented,  // 0x89
  not_implemented,  // 0x8a
  not_implemented,  // 0x8b
  not_implemented,  // 0x8c
  not_implemented,  // 0x8d
  not_implemented,  // 0x8e
  not_implemented,  // 0x8f
  not_implemented,  // 0x90
  not_implemented,  // 0x91
  not_implemented,  // 0x92
  not_implemented,  // 0x93
  not_implemented,  // 0x94
  not_implemented,  // 0x95
  not_implemented,  // 0x96
  not_implemented,  // 0x97
  not_implemented,  // 0x98
  not_implemented,  // 0x99
  not_implemented,  // 0x9a
  not_implemented,  // 0x9b
  not_implemented,  // 0x9c
  not_implemented,  // 0x9d
  not_implemented,  // 0x9e
  not_implemented,  // 0x9f
  not_implemented,  // 0xa0
  not_implemented,  // 0xa1
  not_implemented,  // 0xa2
  not_implemented,  // 0xa3
  not_implemented,  // 0xa4
  not_implemented,  // 0xa5
  not_implemented,  // 0xa6
  not_implemented,  // 0xa7
  XOR_B,            // 0xa8
  XOR_C,            // 0xa9
  XOR_D,            // 0xaa
  XOR_E,            // 0xab
  XOR_H,            // 0xac
  XOR_L,            // 0xad
  not_implemented,  // 0xae
  XOR_A,            // 0xaf
  not_implemented,  // 0xb0
  not_implemented,  // 0xb1
  not_implemented,  // 0xb2
  not_implemented,  // 0xb3
  not_implemented,  // 0xb4
  not_implemented,  // 0xb5
  not_implemented,  // 0xb6
  not_implemented,  // 0xb7
  not_implemented,  // 0xb8
  not_implemented,  // 0xb9
  not_implemented,  // 0xba
  not_implemented,  // 0xbb
  not_implemented,  // 0xbc
  not_implemented,  // 0xbd
  not_implemented,  // 0xbe
  not_implemented,  // 0xbf
  not_implemented,  // 0xc0
  POP_BC,           // 0xc1
  JP_NZ,            // 0xc2
  JP,               // 0xc3
  not_implemented,  // 0xc4
  PUSH_BC,          // 0xc5
  not_implemented,  // 0xc6
  RST_00,           // 0xc7
  not_implemented,  // 0xc8
  not_implemented,  // 0xc9
  JP_Z,             // 0xca
  not_implemented,  // 0xcb
  not_implemented,  // 0xcc
  not_implemented,  // 0xcd
  not_implemented,  // 0xce
  RST_08,           // 0xcf
  not_implemented,  // 0xd0
  POP_DE,           // 0xd1
  JP_NC,            // 0xd2
  not_implemented,  // 0xd3
  not_implemented,  // 0xd4
  PUSH_DE,          // 0xd5
  not_implemented,  // 0xd6
  RST_10,           // 0xd7
  not_implemented,  // 0xd8
  not_implemented,  // 0xd9
  JP_C,             // 0xda
  not_implemented,  // 0xdb
  not_implemented,  // 0xdc
  not_implemented,  // 0xdd
  not_implemented,  // 0xde
  RST_18,           // 0xdf
  not_implemented,  // 0xe0
  POP_HL,           // 0xe1
  not_implemented,  // 0xe2
  not_implemented,  // 0xe3
  not_implemented,  // 0xe4
  PUSH_HL,          // 0xe5
  not_implemented,  // 0xe6
  RST_20,           // 0xe7
  not_implemented,  // 0xe8
  not_implemented,  // 0xe9
  not_implemented,  // 0xea
  not_implemented,  // 0xeb
  not_implemented,  // 0xec
  not_implemented,  // 0xed
  not_implemented,  // 0xee
  RST_28,           // 0xef
  not_implemented,  // 0xf0
  POP_AF,           // 0xf1
  not_implemented,  // 0xf2
  not_implemented,  // 0xf3
  not_implemented,  // 0xf4
  PUSH_AF,          // 0xf5
  not_implemented,  // 0xf6
  RST_30,           // 0xf7
  not_implemented,  // 0xf8
  not_implemented,  // 0xf9
  not_implemented,  // 0xfa
  not_implemented,  // 0xfb
  not_implemented,  // 0xfc
  not_implemented,  // 0xfd
  not_implemented,  // 0xfe
  RST_38,            // 0xff
};

int run_operation(proc* p, uint8_t op)
{
  return operations[op](p); 
}

