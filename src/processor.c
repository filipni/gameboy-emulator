#include "processor.h"

int not_implemented(proc* p) { return -1; }

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

int NOP(proc* p) { p->pc++; return 4; }

op operations[NUM_OPS] = { 
  NOP, // 0x00
  not_implemented, // 0x01
  not_implemented, // 0x02
  not_implemented, // 0x03
  not_implemented, // 0x04
  not_implemented, // 0x05
  not_implemented, // 0x06
  not_implemented, // 0x07
  not_implemented, // 0x08
  not_implemented, // 0x09
  not_implemented, // 0x0a
  not_implemented, // 0x0b
  not_implemented, // 0x0c
  not_implemented, // 0x0d
  not_implemented, // 0x0e
  not_implemented, // 0x0f
  not_implemented, // 0x10
  not_implemented, // 0x11
  not_implemented, // 0x12
  not_implemented, // 0x13
  not_implemented, // 0x14
  not_implemented, // 0x15
  not_implemented, // 0x16
  not_implemented, // 0x17
  not_implemented, // 0x18
  not_implemented, // 0x19
  not_implemented, // 0x1a
  not_implemented, // 0x1b
  not_implemented, // 0x1c
  not_implemented, // 0x1d
  not_implemented, // 0x1e
  not_implemented, // 0x1f
  not_implemented, // 0x20
  not_implemented, // 0x21
  not_implemented, // 0x22
  not_implemented, // 0x23
  not_implemented, // 0x24
  not_implemented, // 0x25
  not_implemented, // 0x26
  not_implemented, // 0x27
  not_implemented, // 0x28
  not_implemented, // 0x29
  not_implemented, // 0x2a
  not_implemented, // 0x2b
  not_implemented, // 0x2c
  not_implemented, // 0x2d
  not_implemented, // 0x2e
  not_implemented, // 0x2f
  not_implemented, // 0x30
  not_implemented, // 0x31
  not_implemented, // 0x32
  not_implemented, // 0x33
  not_implemented, // 0x34
  not_implemented, // 0x35
  not_implemented, // 0x36
  not_implemented, // 0x37
  not_implemented, // 0x38
  not_implemented, // 0x39
  not_implemented, // 0x3a
  not_implemented, // 0x3b
  not_implemented, // 0x3c
  not_implemented, // 0x3d
  not_implemented, // 0x3e
  not_implemented, // 0x3f
  not_implemented, // 0x40
  not_implemented, // 0x41
  not_implemented, // 0x42
  not_implemented, // 0x43
  not_implemented, // 0x44
  not_implemented, // 0x45
  not_implemented, // 0x46
  not_implemented, // 0x47
  not_implemented, // 0x48
  not_implemented, // 0x49
  not_implemented, // 0x4a
  not_implemented, // 0x4b
  not_implemented, // 0x4c
  not_implemented, // 0x4d
  not_implemented, // 0x4e
  not_implemented, // 0x4f
  not_implemented, // 0x50
  not_implemented, // 0x51
  not_implemented, // 0x52
  not_implemented, // 0x53
  not_implemented, // 0x54
  not_implemented, // 0x55
  not_implemented, // 0x56
  not_implemented, // 0x57
  not_implemented, // 0x58
  not_implemented, // 0x59
  not_implemented, // 0x5a
  not_implemented, // 0x5b
  not_implemented, // 0x5c
  not_implemented, // 0x5d
  not_implemented, // 0x5e
  not_implemented, // 0x5f
  not_implemented, // 0x60
  not_implemented, // 0x61
  not_implemented, // 0x62
  not_implemented, // 0x63
  not_implemented, // 0x64
  not_implemented, // 0x65
  not_implemented, // 0x66
  not_implemented, // 0x67
  not_implemented, // 0x68
  not_implemented, // 0x69
  not_implemented, // 0x6a
  not_implemented, // 0x6b
  not_implemented, // 0x6c
  not_implemented, // 0x6d
  not_implemented, // 0x6e
  not_implemented, // 0x6f
  not_implemented, // 0x70
  not_implemented, // 0x71
  not_implemented, // 0x72
  not_implemented, // 0x73
  not_implemented, // 0x74
  not_implemented, // 0x75
  not_implemented, // 0x76
  not_implemented, // 0x77
  not_implemented, // 0x78
  not_implemented, // 0x79
  not_implemented, // 0x7a
  not_implemented, // 0x7b
  not_implemented, // 0x7c
  not_implemented, // 0x7d
  not_implemented, // 0x7e
  not_implemented, // 0x7f
  not_implemented, // 0x80
  not_implemented, // 0x81
  not_implemented, // 0x82
  not_implemented, // 0x83
  not_implemented, // 0x84
  not_implemented, // 0x85
  not_implemented, // 0x86
  not_implemented, // 0x87
  not_implemented, // 0x88
  not_implemented, // 0x89
  not_implemented, // 0x8a
  not_implemented, // 0x8b
  not_implemented, // 0x8c
  not_implemented, // 0x8d
  not_implemented, // 0x8e
  not_implemented, // 0x8f
  not_implemented, // 0x90
  not_implemented, // 0x91
  not_implemented, // 0x92
  not_implemented, // 0x93
  not_implemented, // 0x94
  not_implemented, // 0x95
  not_implemented, // 0x96
  not_implemented, // 0x97
  not_implemented, // 0x98
  not_implemented, // 0x99
  not_implemented, // 0x9a
  not_implemented, // 0x9b
  not_implemented, // 0x9c
  not_implemented, // 0x9d
  not_implemented, // 0x9e
  not_implemented, // 0x9f
  not_implemented, // 0xa0
  not_implemented, // 0xa1
  not_implemented, // 0xa2
  not_implemented, // 0xa3
  not_implemented, // 0xa4
  not_implemented, // 0xa5
  not_implemented, // 0xa6
  not_implemented, // 0xa7
  not_implemented, // 0xa8
  not_implemented, // 0xa9
  not_implemented, // 0xaa
  not_implemented, // 0xab
  not_implemented, // 0xac
  not_implemented, // 0xad
  not_implemented, // 0xae
  not_implemented, // 0xaf
  not_implemented, // 0xb0
  not_implemented, // 0xb1
  not_implemented, // 0xb2
  not_implemented, // 0xb3
  not_implemented, // 0xb4
  not_implemented, // 0xb5
  not_implemented, // 0xb6
  not_implemented, // 0xb7
  not_implemented, // 0xb8
  not_implemented, // 0xb9
  not_implemented, // 0xba
  not_implemented, // 0xbb
  not_implemented, // 0xbc
  not_implemented, // 0xbd
  not_implemented, // 0xbe
  not_implemented, // 0xbf
  not_implemented, // 0xc0
  not_implemented, // 0xc1
  not_implemented, // 0xc2
  not_implemented, // 0xc3
  not_implemented, // 0xc4
  not_implemented, // 0xc5
  not_implemented, // 0xc6
  not_implemented, // 0xc7
  not_implemented, // 0xc8
  not_implemented, // 0xc9
  not_implemented, // 0xca
  not_implemented, // 0xcb
  not_implemented, // 0xcc
  not_implemented, // 0xcd
  not_implemented, // 0xce
  not_implemented, // 0xcf
  not_implemented, // 0xd0
  not_implemented, // 0xd1
  not_implemented, // 0xd2
  not_implemented, // 0xd3
  not_implemented, // 0xd4
  not_implemented, // 0xd5
  not_implemented, // 0xd6
  not_implemented, // 0xd7
  not_implemented, // 0xd8
  not_implemented, // 0xd9
  not_implemented, // 0xda
  not_implemented, // 0xdb
  not_implemented, // 0xdc
  not_implemented, // 0xdd
  not_implemented, // 0xde
  not_implemented, // 0xdf
  not_implemented, // 0xe0
  not_implemented, // 0xe1
  not_implemented, // 0xe2
  not_implemented, // 0xe3
  not_implemented, // 0xe4
  not_implemented, // 0xe5
  not_implemented, // 0xe6
  not_implemented, // 0xe7
  not_implemented, // 0xe8
  not_implemented, // 0xe9
  not_implemented, // 0xea
  not_implemented, // 0xeb
  not_implemented, // 0xec
  not_implemented, // 0xed
  not_implemented, // 0xee
  not_implemented, // 0xef
  not_implemented, // 0xf0
  not_implemented, // 0xf1
  not_implemented, // 0xf2
  not_implemented, // 0xf3
  not_implemented, // 0xf4
  not_implemented, // 0xf5
  not_implemented, // 0xf6
  not_implemented, // 0xf7
  not_implemented, // 0xf8
  not_implemented, // 0xf9
  not_implemented, // 0xfa
  not_implemented, // 0xfb
  not_implemented, // 0xfc
  not_implemented, // 0xfd
  not_implemented, // 0xfe
  not_implemented, // 0xff
};

int run_operation(proc* p, uint8_t op)
{
  return operations[op](p); 
}

