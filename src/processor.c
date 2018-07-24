#include "processor.h"
#include "utils.h"

int not_implemented(proc* p) { return -1; }

int LD_d8(proc* p, uint8_t* nn)
{
  *nn = p->mem[p->pc+1];
 
  p->pc += 2;
  return 8;
}

int LD_B(proc* p) { LD_d8(p, &p->bc.r8.high); }
int LD_D(proc* p) { LD_d8(p, &p->de.r8.high); }
int LD_H(proc* p) { LD_d8(p, &p->hl.r8.high); }

int LD_C(proc* p) { LD_d8(p, &p->bc.r8.low); }
int LD_E(proc* p) { LD_d8(p, &p->de.r8.low); }
int LD_L(proc* p) { LD_d8(p, &p->hl.r8.low); }
int LD_A(proc* p) { LD_d8(p, &p->af.r8.high); }

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
// LD_BHL
int LD_B_A(proc* p) { return LD_reg(p, &p->bc.r8.high, &p->af.r8.high); }

int LD_C_B(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->bc.r8.high); }
int LD_C_C(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->bc.r8.low); }
int LD_C_D(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->de.r8.high); }
int LD_C_E(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->de.r8.low); }
int LD_C_H(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->hl.r8.high); }
int LD_C_L(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->hl.r8.low); }
// LD_CHL
int LD_C_A(proc* p) { return LD_reg(p, &p->bc.r8.low, &p->af.r8.high); }

int LD_D_B(proc* p) { return LD_reg(p, &p->de.r8.high, &p->bc.r8.high); }
int LD_D_C(proc* p) { return LD_reg(p, &p->de.r8.high, &p->bc.r8.low); }
int LD_D_D(proc* p) { return LD_reg(p, &p->de.r8.high, &p->de.r8.high); }
int LD_D_E(proc* p) { return LD_reg(p, &p->de.r8.high, &p->de.r8.low); }
int LD_D_H(proc* p) { return LD_reg(p, &p->de.r8.high, &p->hl.r8.high); }
int LD_D_L(proc* p) { return LD_reg(p, &p->de.r8.high, &p->hl.r8.low); }
// LD_DHL
int LD_D_A(proc* p) { return LD_reg(p, &p->de.r8.high, &p->af.r8.high); }

int LD_E_B(proc* p) { return LD_reg(p, &p->de.r8.low, &p->bc.r8.high); }
int LD_E_C(proc* p) { return LD_reg(p, &p->de.r8.low, &p->bc.r8.low); }
int LD_E_D(proc* p) { return LD_reg(p, &p->de.r8.low, &p->de.r8.high); }
int LD_E_E(proc* p) { return LD_reg(p, &p->de.r8.low, &p->de.r8.low); }
int LD_E_H(proc* p) { return LD_reg(p, &p->de.r8.low, &p->hl.r8.high); }
int LD_E_L(proc* p) { return LD_reg(p, &p->de.r8.low, &p->hl.r8.low); }
// LD_EHL
int LD_E_A(proc* p) { return LD_reg(p, &p->de.r8.low, &p->af.r8.high); }

int LD_H_B(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->bc.r8.high); }
int LD_H_C(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->bc.r8.low); }
int LD_H_D(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->de.r8.high); }
int LD_H_E(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->de.r8.low); }
int LD_H_H(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->hl.r8.high); }
int LD_H_L(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->hl.r8.low); }
// LD_EHL
int LD_H_A(proc* p) { return LD_reg(p, &p->hl.r8.high, &p->af.r8.high); }

int LD_L_B(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->bc.r8.high); }
int LD_L_C(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->bc.r8.low); }
int LD_L_D(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->de.r8.high); }
int LD_L_E(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->de.r8.low); }
int LD_L_H(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->hl.r8.high); }
int LD_L_L(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->hl.r8.low); }
// LD_EHL
int LD_L_A(proc* p) { return LD_reg(p, &p->hl.r8.low, &p->af.r8.high); }

int LD_HL(proc* p)
{
  p->hl.r8.low = p->mem[p->pc+1]; 
  p->hl.r8.high = p->mem[p->pc+2]; 

  p->pc += 3;
  return 12;
}

int NOP(proc* p) { p->pc++; return 4; }

int JP(proc* p)
{
  uint16_t adr = generate_address(p->mem[p->pc+1], p->mem[p->pc+2]);
  p->pc = adr;
  return 16;
}

int XOR(proc* p, uint8_t* reg, uint8_t arg)
{
  *reg |= arg;
  
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

op operations[NUM_OPS] = { 
  NOP,              // 0x00
  not_implemented,  // 0x01
  not_implemented,  // 0x02
  not_implemented,  // 0x03
  not_implemented,  // 0x04
  not_implemented,  // 0x05
  LD_B,             // 0x06
  not_implemented,  // 0x07
  not_implemented,  // 0x08
  not_implemented,  // 0x09
  not_implemented,  // 0x0a
  not_implemented,  // 0x0b
  not_implemented,  // 0x0c
  not_implemented,  // 0x0d
  LD_C,             // 0x0e
  not_implemented,  // 0x0f
  not_implemented,  // 0x10
  not_implemented,  // 0x11
  not_implemented,  // 0x12
  not_implemented,  // 0x13
  not_implemented,  // 0x14
  not_implemented,  // 0x15
  LD_D,             // 0x16
  not_implemented,  // 0x17
  not_implemented,  // 0x18
  not_implemented,  // 0x19
  not_implemented,  // 0x1a
  not_implemented,  // 0x1b
  not_implemented,  // 0x1c
  not_implemented,  // 0x1d
  LD_E,             // 0x1e
  not_implemented,  // 0x1f
  not_implemented,  // 0x20
  LD_HL,            // 0x21
  not_implemented,  // 0x22
  not_implemented,  // 0x23
  not_implemented,  // 0x24
  not_implemented,  // 0x25
  LD_H,             // 0x26
  not_implemented,  // 0x27
  not_implemented,  // 0x28
  not_implemented,  // 0x29
  not_implemented,  // 0x2a
  not_implemented,  // 0x2b
  not_implemented,  // 0x2c
  not_implemented,  // 0x2d
  LD_L,             // 0x2e
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
  LD_A,             // 0x3e
  not_implemented,  // 0x3f
  LD_B_B,           // 0x40
  LD_B_C,           // 0x41
  LD_B_D,           // 0x42
  LD_B_E,           // 0x43
  LD_B_H,           // 0x44
  LD_B_L,           // 0x45
  not_implemented,  // 0x46
  LD_B_A,           // 0x47
  LD_C_B,           // 0x48
  LD_C_C,           // 0x49
  LD_C_D,           // 0x4a
  LD_C_E,           // 0x4b
  LD_C_H,           // 0x4c
  LD_C_L,           // 0x4d
  not_implemented,  // 0x4e
  LD_C_A,           // 0x4f
  LD_D_B,           // 0x50
  LD_D_C,           // 0x51
  LD_D_D,           // 0x52
  LD_D_E,           // 0x53
  LD_D_H,           // 0x54
  LD_D_L,           // 0x55
  not_implemented,  // 0x56
  LD_D_A,           // 0x57 
  LD_E_B,           // 0x58
  LD_E_C,           // 0x59
  LD_E_D,           // 0x5a
  LD_E_E,           // 0x5b
  LD_E_H,           // 0x5c
  LD_E_L,           // 0x5d
  not_implemented,  // 0x5e
  LD_E_A,           // 0x5f 
  LD_H_B,           // 0x60
  LD_H_C,           // 0x61
  LD_H_D,           // 0x62
  LD_H_E,           // 0x63
  LD_H_H,           // 0x64
  LD_H_L,           // 0x65
  not_implemented,  // 0x66
  LD_H_A,           // 0x67 
  LD_L_B,           // 0x68
  LD_L_C,           // 0x69
  LD_L_D,           // 0x6a
  LD_L_E,           // 0x6b
  LD_L_H,           // 0x6c
  LD_L_L,           // 0x6d
  not_implemented,  // 0x6e
  LD_L_A,           // 0x6f 
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
  not_implemented,  // 0xc1
  not_implemented,  // 0xc2
  JP,               // 0xc3
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
};

int run_operation(proc* p, uint8_t op)
{
  return operations[op](p); 
}

