#include "processor.h"
#include "utils.h"
#include <string.h>

int not_implemented(proc* p) { return -1; }

int test_bit(proc* p, uint8_t* r, uint8_t bit)
{
  uint8_t bitmask = 1 << bit;
  set_flag(p, ZERO, *r & bitmask);
  clear_flags(p, SUBTRACT);
  set_flag(p, HALF_CARRY, 1);

  p->pc += 2;
  return 8;
}

int set_bit(proc* p, uint8_t* r, uint8_t bit)
{
  uint8_t bitmask = 1 << bit;
  *r |= bitmask;

  p->pc += 2;
  return 8;
}

int reset_bit(proc* p, uint8_t* r, uint8_t bit)
{
  uint8_t bitmask = 1 << bit;
  *r &= ~bitmask;

  p->pc += 2;
  return 8;
}

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

int DI(proc* p)
{
  p->interrupts_enabled = 0; 

  p->pc++;
  return 4;
}

int EI(proc* p)
{
  p->interrupts_enabled = 1; 

  p->pc++;
  return 4;
}

int INC_r8(proc* p, uint8_t* r)
{
  set_flag(p, HALF_CARRY, calculate_half_carry(*r, 1));
  
  (*r)++;

  if (*r == 0)
    set_flag(p, ZERO, 1);
  else
    clear_flags(p, ZERO);
  clear_flags(p, SUBTRACT);

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
  set_flag(p, HALF_CARRY, *r < 1);
  
  (*r)--;

  if (*r == 0)
    set_flag(p, ZERO, 1);
  else
    clear_flags(p, ZERO);
  set_flag(p, SUBTRACT, 1);

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

int LD_B(proc* p) { return LD_d8(p, &p->bc.r8.high); }
int LD_D(proc* p) { return LD_d8(p, &p->de.r8.high); }
int LD_H(proc* p) { return LD_d8(p, &p->hl.r8.high); }
int LD_mHL(proc* p) { return LD_d8(p, (uint8_t*) (p->mem + p->hl.r16)); }

int LD_C(proc* p) { return LD_d8(p, &p->bc.r8.low); }
int LD_E(proc* p) { return LD_d8(p, &p->de.r8.low); }
int LD_L(proc* p) { return LD_d8(p, &p->hl.r8.low); }
int LD_A(proc* p) { return LD_d8(p, &p->af.r8.high); }

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

int JP_NZ(proc* p) { return COND_JP(p, !test_flag(p, ZERO)); }
int JP_Z(proc* p) { return COND_JP(p, test_flag(p, ZERO)); }
int JP_NC(proc* p) { return COND_JP(p, !test_flag(p, CARRY)); }
int JP_C(proc* p) { return COND_JP(p, test_flag(p, CARRY)); }

int JR(proc* p)
{
  uint16_t adr = p->pc + 1 + p->mem[p->pc+1];
  p->pc = adr;
  return 12;
}

int COND_JR(proc* p, uint8_t cond)
{
  if (cond)
  {
    uint16_t adr = p->pc + 2 + p->mem[p->pc+1];
    p->pc = adr;
    return 12;
  }

  p->pc += 2;
  return 8; 
}

int JR_NZ(proc* p) { return COND_JR(p, !test_flag(p, ZERO)); }
int JR_Z(proc* p) { return COND_JR(p, test_flag(p, ZERO)); }
int JR_NC(proc* p) { return COND_JR(p, !test_flag(p, CARRY)); }
int JR_C(proc* p) { return COND_JR(p, test_flag(p, CARRY)); }

int XOR(proc* p, uint8_t* reg)
{
  p->af.r8.high ^= *reg ;

  // set flags
  clear_flags(p, ZERO | SUBTRACT | HALF_CARRY | CARRY);
  set_flag(p, ZERO, !p->af.r8.high);
  
  p->pc++;
  return 4;
}

int XOR_B(proc* p) { return XOR(p, &p->bc.r8.high); }
int XOR_C(proc* p) { return XOR(p, &p->bc.r8.low); }
int XOR_D(proc* p) { return XOR(p, &p->de.r8.high); }
int XOR_E(proc* p) { return XOR(p, &p->de.r8.low); }
int XOR_H(proc* p) { return XOR(p, &p->hl.r8.high); }
int XOR_L(proc* p) { return XOR(p, &p->hl.r8.low); }
int XOR_A(proc* p) { return XOR(p, &p->af.r8.high); }

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

  uint8_t res = *r1 + r2 + carry;
  int carrybits = *r1 ^ r2 ^ res; 

  clear_flags(p, SUBTRACT);
  set_flag(p, ZERO, !res);
  set_flag(p, HALF_CARRY, carrybits & 0x10 == 1);
  set_flag(p, CARRY, carrybits & 0x100 == 1); 

  *r1 = res;

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

int ADC_A_B(proc* p) { return ADD(p, &p->af.r8.high, p->bc.r8.high, test_flag(p, CARRY)); } 
int ADC_A_C(proc* p) { return ADD(p, &p->af.r8.high, p->bc.r8.low, test_flag(p, CARRY)); } 
int ADC_A_D(proc* p) { return ADD(p, &p->af.r8.high, p->de.r8.high, test_flag(p, CARRY)); } 
int ADC_A_E(proc* p) { return ADD(p, &p->af.r8.high, p->de.r8.low, test_flag(p, CARRY)); } 
int ADC_A_H(proc* p) { return ADD(p, &p->af.r8.high, p->hl.r8.high, test_flag(p, CARRY)); } 
int ADC_A_L(proc* p) { return ADD(p, &p->af.r8.high, p->hl.r8.low, test_flag(p, CARRY)); } 
int ADC_A_mHL(proc* p) { return ADD(p, &p->af.r8.high, p->mem[p->hl.r16], test_flag(p, CARRY)); }
int ADC_A_A(proc* p) { return ADD(p, &p->af.r8.high, p->af.r8.high, test_flag(p, CARRY)); }

int SUB(proc* p, uint8_t* r1, uint8_t r2, int carry)
{
  set_flag(p, SUBTRACT, 1);
  set_flag(p, CARRY, r2 + carry > *r1);
  set_flag(p, HALF_CARRY, (r2 + carry & 0x0F) > (*r1 & 0x0F)  ); 

  *r1 -= r2 + carry;
  set_flag(p, ZERO, !(*r1));

  p->pc++;
  return 4;
}

int SUB_B(proc* p) { return SUB(p, &p->af.r8.high, p->bc.r8.high, 0); }
int SUB_C(proc* p) { return SUB(p, &p->af.r8.high, p->bc.r8.low, 0); }
int SUB_D(proc* p) { return SUB(p, &p->af.r8.high, p->de.r8.high, 0); }
int SUB_E(proc* p) { return SUB(p, &p->af.r8.high, p->de.r8.low, 0); }
int SUB_H(proc* p) { return SUB(p, &p->af.r8.high, p->hl.r8.high, 0); }
int SUB_L(proc* p) { return SUB(p, &p->af.r8.high, p->hl.r8.low, 0); }
int SUB_mHL(proc* p) { return SUB(p, &p->af.r8.high, p->mem[p->hl.r16], 0); }
int SUB_A(proc* p) { return SUB(p, &p->af.r8.high, p->af.r8.high, 0); }

int SBC_B(proc* p) { return SUB(p, &p->af.r8.high, p->bc.r8.high, test_flag(p, CARRY)); }
int SBC_C(proc* p) { return SUB(p, &p->af.r8.high, p->bc.r8.low, test_flag(p, CARRY)); }
int SBC_D(proc* p) { return SUB(p, &p->af.r8.high, p->de.r8.high, test_flag(p, CARRY)); }
int SBC_E(proc* p) { return SUB(p, &p->af.r8.high, p->de.r8.low, test_flag(p, CARRY)); }
int SBC_H(proc* p) { return SUB(p, &p->af.r8.high, p->hl.r8.high, test_flag(p, CARRY)); }
int SBC_L(proc* p) { return SUB(p, &p->af.r8.high, p->hl.r8.low, test_flag(p, CARRY)); }
int SBC_mHL(proc* p) { return SUB(p, &p->af.r8.high, p->mem[p->hl.r16], test_flag(p, CARRY)); }
int SBC_A(proc* p) { return SUB(p, &p->af.r8.high, p->af.r8.high, test_flag(p, CARRY)); }

int AND(proc* p, uint8_t* r)
{
  p->af.r8.high &= *r;
  set_flag(p, ZERO, !p->af.r8.high);
  set_flag(p, HALF_CARRY, 1);
  clear_flags(p, SUBTRACT | CARRY);
  
  p->pc++;
  return 4;
}

int AND_B(proc* p) { return AND(p, &p->bc.r8.high); }
int AND_C(proc* p) { return AND(p, &p->bc.r8.low); }
int AND_D(proc* p) { return AND(p, &p->de.r8.high); }
int AND_E(proc* p) { return AND(p, &p->de.r8.low); }
int AND_H(proc* p) { return AND(p, &p->hl.r8.high); }
int AND_L(proc* p) { return AND(p, &p->hl.r8.low); }
int AND_mHL(proc* p) { return AND(p, &p->mem[p->hl.r16]); }
int AND_A(proc* p) { return AND(p, &p->af.r8.high); }

int OR(proc* p, uint8_t* r)
{
  p->af.r8.high |= *r;
  set_flag(p, ZERO, !p->af.r8.high);
  clear_flags(p, SUBTRACT | HALF_CARRY | CARRY);
  
  p->pc++;
  return 4;
}

int OR_B(proc* p) { return OR(p, &p->bc.r8.high); }
int OR_C(proc* p) { return OR(p, &p->bc.r8.low); }
int OR_D(proc* p) { return OR(p, &p->de.r8.high); }
int OR_E(proc* p) { return OR(p, &p->de.r8.low); }
int OR_H(proc* p) { return OR(p, &p->hl.r8.high); }
int OR_L(proc* p) { return OR(p, &p->hl.r8.low); }
int OR_mHL(proc* p) { return OR(p, &p->mem[p->hl.r16]); }
int OR_A(proc* p) { return OR(p, &p->af.r8.high); }

int LDH_a8_A(proc* p)
{
  uint16_t addr = 0xFF00 + p->mem[p->pc+1];
  p->mem[addr] = p->af.r8.high;

  p->pc += 2; 
  return 12;
}

int LDH_A_a8(proc* p)
{
  uint16_t addr = 0xFF00 + p->mem[p->pc+1];
  p->af.r8.high = p->mem[addr];

  p->pc += 2; 
  return 12;
}

int CP(proc* p)
{
  uint8_t n = p->mem[p->pc+1];
  uint8_t a = p->af.r8.high;

  set_flag(p, SUBTRACT, 1);
  set_flag(p, ZERO, a == n);
  set_flag(p, HALF_CARRY, (a & 0xF) < (n & 0xF));
  set_flag(p, CARRY, a < n);

  p->pc += 2;
  return 8;
}

op prefix_operations[NUM_OPS] = {
  not_implemented,  // 0x00
  not_implemented,  // 0x01
  not_implemented,  // 0x02
  not_implemented,  // 0x03
  not_implemented,  // 0x04
  not_implemented,  // 0x05
  not_implemented,  // 0x06
  not_implemented,  // 0x07
  not_implemented,  // 0x08
  not_implemented,  // 0x09
  not_implemented,  // 0x0a
  not_implemented,  // 0x0b
  not_implemented,  // 0x0c
  not_implemented,  // 0x0d
  not_implemented,  // 0x0e
  not_implemented,  // 0x0f
  not_implemented,  // 0x10
  not_implemented,  // 0x11
  not_implemented,  // 0x12
  not_implemented,  // 0x13
  not_implemented,  // 0x14
  not_implemented,  // 0x15
  not_implemented,  // 0x16
  not_implemented,  // 0x17
  not_implemented,  // 0x18
  not_implemented,  // 0x19
  not_implemented,  // 0x1a
  not_implemented,  // 0x1b
  not_implemented,  // 0x1c
  not_implemented,  // 0x1d
  not_implemented,  // 0x1e
  not_implemented,  // 0x1f
  not_implemented,  // 0x20
  not_implemented,  // 0x21
  not_implemented,  // 0x22
  not_implemented,  // 0x23
  not_implemented,  // 0x24
  not_implemented,  // 0x25
  not_implemented,  // 0x26
  not_implemented,  // 0x27
  not_implemented,  // 0x28
  not_implemented,  // 0x29
  not_implemented,  // 0x2a
  not_implemented,  // 0x2b
  not_implemented,  // 0x2c
  not_implemented,  // 0x2d
  not_implemented,  // 0x2e
  not_implemented,  // 0x2f
  not_implemented,  // 0x30
  not_implemented,  // 0x31
  not_implemented,  // 0x32
  not_implemented,  // 0x33
  not_implemented,  // 0x34
  not_implemented,  // 0x35
  not_implemented,  // 0x36
  not_implemented,  // 0x37
  not_implemented,  // 0x38
  not_implemented,  // 0x39
  not_implemented,  // 0x3a
  not_implemented,  // 0x3b
  not_implemented,  // 0x3c
  not_implemented,  // 0x3d
  not_implemented,  // 0x3e
  not_implemented,  // 0x3f
  not_implemented,  // 0x40
  not_implemented,  // 0x41
  not_implemented,  // 0x42
  not_implemented,  // 0x43
  not_implemented,  // 0x44
  not_implemented,  // 0x45
  not_implemented,  // 0x46
  not_implemented,  // 0x47
  not_implemented,  // 0x48
  not_implemented,  // 0x49
  not_implemented,  // 0x4a
  not_implemented,  // 0x4b
  not_implemented,  // 0x4c
  not_implemented,  // 0x4d
  not_implemented,  // 0x4e
  not_implemented,  // 0x4f
  not_implemented,  // 0x50
  not_implemented,  // 0x51
  not_implemented,  // 0x52
  not_implemented,  // 0x53
  not_implemented,  // 0x54
  not_implemented,  // 0x55
  not_implemented,  // 0x56
  not_implemented,  // 0x57
  not_implemented,  // 0x58
  not_implemented,  // 0x59
  not_implemented,  // 0x5a
  not_implemented,  // 0x5b
  not_implemented,  // 0x5c
  not_implemented,  // 0x5d
  not_implemented,  // 0x5e
  not_implemented,  // 0x5f
  not_implemented,  // 0x60
  not_implemented,  // 0x61
  not_implemented,  // 0x62
  not_implemented,  // 0x63
  not_implemented,  // 0x64
  not_implemented,  // 0x65
  not_implemented,  // 0x66
  not_implemented,  // 0x67
  not_implemented,  // 0x68
  not_implemented,  // 0x69
  not_implemented,  // 0x6a
  not_implemented,  // 0x6b
  not_implemented,  // 0x6c
  not_implemented,  // 0x6d
  not_implemented,  // 0x6e
  not_implemented,  // 0x6f
  not_implemented,  // 0x70
  not_implemented,  // 0x71
  not_implemented,  // 0x72
  not_implemented,  // 0x73
  not_implemented,  // 0x74
  not_implemented,  // 0x75
  not_implemented,  // 0x76
  not_implemented,  // 0x77
  not_implemented,  // 0x78
  not_implemented,  // 0x79
  not_implemented,  // 0x7a
  not_implemented,  // 0x7b
  not_implemented,  // 0x7c
  not_implemented,  // 0x7d
  not_implemented,  // 0x7e
  not_implemented,  // 0x7f
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
  not_implemented,  // 0xa8
  not_implemented,  // 0xa9
  not_implemented,  // 0xaa
  not_implemented,  // 0xab
  not_implemented,  // 0xac
  not_implemented,  // 0xad
  not_implemented,  // 0xae
  not_implemented,  // 0xaf
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
  not_implemented,  // 0xc1
  not_implemented,  // 0xc2
  not_implemented,  // 0xc3
  not_implemented,  // 0xc4
  not_implemented,  // 0xc5
  not_implemented,  // 0xc6
  not_implemented,  // 0xc7
  not_implemented,  // 0xc8
  not_implemented,  // 0xc9
  not_implemented,  // 0xca
  not_implemented,  // 0xcb
  not_implemented,  // 0xcc
  not_implemented,  // 0xcd
  not_implemented,  // 0xce
  not_implemented,  // 0xcf
  not_implemented,  // 0xd0
  not_implemented,  // 0xd1
  not_implemented,  // 0xd2
  not_implemented,  // 0xd3
  not_implemented,  // 0xd4
  not_implemented,  // 0xd5
  not_implemented,  // 0xd6
  not_implemented,  // 0xd7
  not_implemented,  // 0xd8
  not_implemented,  // 0xd9
  not_implemented,  // 0xda
  not_implemented,  // 0xdb
  not_implemented,  // 0xdc
  not_implemented,  // 0xdd
  not_implemented,  // 0xde
  not_implemented,  // 0xdf
  not_implemented,  // 0xe0
  not_implemented,  // 0xe1
  not_implemented,  // 0xe2
  not_implemented,  // 0xe3
  not_implemented,  // 0xe4
  not_implemented,  // 0xe5
  not_implemented,  // 0xe6
  not_implemented,  // 0xe7
  not_implemented,  // 0xe8
  not_implemented,  // 0xe9
  not_implemented,  // 0xea
  not_implemented,  // 0xeb
  not_implemented,  // 0xec
  not_implemented,  // 0xed
  not_implemented,  // 0xee
  not_implemented,  // 0xef
  not_implemented,  // 0xf0
  not_implemented,  // 0xf1
  not_implemented,  // 0xf2
  not_implemented,  // 0xf3
  not_implemented,  // 0xf4
  not_implemented,  // 0xf5
  not_implemented,  // 0xf6
  not_implemented,  // 0xf7
  not_implemented,  // 0xf8
  not_implemented,  // 0xf9
  not_implemented,  // 0xfa
  not_implemented,  // 0xfb
  not_implemented,  // 0xfc
  not_implemented,  // 0xfd
  not_implemented,  // 0xfe
  not_implemented,  // 0xff
}

op operations[NUM_OPS] = { 
  NOP,              // 0x00
  LD_BC,            // 0x01
  LD_mBC_A,         // 0x02
  INC_BC,           // 0x03
  INC_B,            // 0x04
  DEC_B,            // 0x05
  LD_B,             // 0x06
  not_implemented,  // 0x07
  not_implemented,  // 0x08
  not_implemented,  // 0x09
  LD_A_mBC,         // 0x0a
  DEC_BC,           // 0x0b
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
  JR,               // 0x18
  not_implemented,  // 0x19
  LD_A_mDE,         // 0x1a
  DEC_DE,           // 0x1b
  INC_E,            // 0x1c
  DEC_E,            // 0x1d
  LD_E,             // 0x1e
  not_implemented,  // 0x1f
  JR_NZ,            // 0x20
  LD_HL,            // 0x21
  LD_mHL_A_inc,     // 0x22
  INC_HL,           // 0x23
  INC_H,            // 0x24
  DEC_H,            // 0x25
  LD_H,             // 0x26
  not_implemented,  // 0x27
  JR_Z,             // 0x28
  not_implemented,  // 0x29
  LD_A_mHL_inc,     // 0x2a
  not_implemented,  // 0x2b
  INC_L,            // 0x2c
  DEC_L,            // 0x2d
  LD_L,             // 0x2e
  not_implemented,  // 0x2f
  JR_NC,            // 0x30
  LD_SP,            // 0x31
  LD_mHL_A_dec,     // 0x32
  INC_SP,           // 0x33
  INC_mHL,          // 0x34
  DEC_mHL,          // 0x35
  LD_mHL,           // 0x36
  not_implemented,  // 0x37
  JR_C,             // 0x38
  not_implemented,  // 0x39
  LD_A_mHL_dec,     // 0x3a
  DEC_SP,           // 0x3b
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
  ADD_A_B,          // 0x80
  ADD_A_C,          // 0x81
  ADD_A_D,          // 0x82
  ADD_A_E,          // 0x83
  ADD_A_H,          // 0x84
  ADD_A_L,          // 0x85
  ADD_A_mHL,        // 0x86
  ADC_A_A,          // 0x87
  ADC_A_B,          // 0x88
  ADC_A_C,          // 0x89
  ADC_A_D,          // 0x8a
  ADC_A_E,          // 0x8b
  ADC_A_H,          // 0x8c
  ADC_A_L,          // 0x8d
  ADC_A_mHL,        // 0x8e
  ADC_A_A,          // 0x8f
  SUB_B,            // 0x90
  SUB_C,            // 0x91
  SUB_D,            // 0x92
  SUB_E,            // 0x93
  SUB_H,            // 0x94
  SUB_L,            // 0x95
  SUB_mHL,          // 0x96
  SUB_A,            // 0x97
  SBC_B,            // 0x98
  SBC_C,            // 0x99
  SBC_D,            // 0x9a
  SBC_E,            // 0x9b
  SBC_H,            // 0x9c
  SBC_L,            // 0x9d
  SBC_mHL,          // 0x9e
  SBC_A,            // 0x9f
  AND_B,            // 0xa0
  AND_C,            // 0xa1
  AND_D,            // 0xa2
  AND_E,            // 0xa3
  AND_H,            // 0xa4
  AND_L,            // 0xa5
  AND_mHL,          // 0xa6
  AND_A,            // 0xa7
  XOR_B,            // 0xa8
  XOR_C,            // 0xa9
  XOR_D,            // 0xaa
  XOR_E,            // 0xab
  XOR_H,            // 0xac
  XOR_L,            // 0xad
  not_implemented,  // 0xae
  XOR_A,            // 0xaf
  OR_B,             // 0xb0
  OR_C,             // 0xb1
  OR_D,             // 0xb2
  OR_E,             // 0xb3
  OR_H,             // 0xb4
  OR_L,             // 0xb5
  OR_mHL,           // 0xb6
  OR_A,             // 0xb7
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
  LDH_a8_A,         // 0xe0
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
  LDH_A_a8,         // 0xf0
  POP_AF,           // 0xf1
  not_implemented,  // 0xf2
  DI,               // 0xf3
  not_implemented,  // 0xf4
  PUSH_AF,          // 0xf5
  not_implemented,  // 0xf6
  RST_30,           // 0xf7
  not_implemented,  // 0xf8
  not_implemented,  // 0xf9
  not_implemented,  // 0xfa
  EI,               // 0xfb
  not_implemented,  // 0xfc
  not_implemented,  // 0xfd
  CP,               // 0xfe
  RST_38,           // 0xff
};

int run_operation(proc* p, uint8_t op)
{
  return operations[op](p); 
}

