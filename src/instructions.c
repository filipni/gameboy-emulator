#include "instructions.h"
#include "utils.h"
#include "memory.h"

#include <stdio.h>

int not_implemented() { return -1; }

int STOP()
{
  printf("STOP is not implemented yet...\n");
  p.pc++;
  return 4;
}

int RET_INT(uint8_t enable_interrupts)
{
  uint16_t addr = generate_address(read_from_mem(p.sp), read_from_mem(p.sp+1));

  if (enable_interrupts)
    p.interrupts_enabled = 1;

  p.sp += 2;
  p.pc = addr;
  return 16;
}

int CALL()
{
  uint16_t next_addr = p.pc + 3;
  // Push address to stack in little-endian order
  write_to_mem(p.sp-1, (next_addr & 0xFF00) >> 8); // High byte
  write_to_mem(p.sp-2, next_addr & 0xFF); // Low byte
  p.sp -= 2;

  uint16_t addr = generate_address(read_from_mem(p.pc+1), read_from_mem(p.pc+2));

  p.pc = addr;
  return 24;
}

int COND_CALL(uint8_t cond)
{
  if (cond)
  {
    uint16_t next_addr = p.pc + 3;
    // Push address to stack in little-endian order
    write_to_mem(p.sp-1, (next_addr & 0xFF00) >> 8); // High byte
    write_to_mem(p.sp-2, next_addr & 0xFF); // Low byte
    p.sp -= 2;

    uint16_t addr = generate_address(read_from_mem(p.pc+1), read_from_mem(p.pc+2));

    p.pc = addr;
    return 24;
  }

  p.pc += 3;
  return 12;
}

int CALL_Z() { return COND_CALL(test_flag(&p, ZERO)); };
int CALL_NZ() { return COND_CALL(!test_flag(&p, ZERO)); };
int CALL_C() { return COND_CALL(test_flag(&p, CARRY)); };
int CALL_NC() { return COND_CALL(!test_flag(&p, CARRY)); };

int RET() { return RET_INT(0); }
int RETI() { return RET_INT(1); }

int COND_RET(uint8_t cond)
{
  if (cond)
  {
    uint16_t addr = generate_address(read_from_mem(p.sp), read_from_mem(p.sp+1));

    p.sp += 2;
    p.pc = addr;
    return 20;
  }

  p.pc++;
  return 8;
}

int RET_Z() { return COND_RET(test_flag(&p, ZERO)); }
int RET_NZ() { return COND_RET(!test_flag(&p, ZERO)); }
int RET_C() { return COND_RET(test_flag(&p, CARRY)); }
int RET_NC() { return COND_RET(!test_flag(&p, CARRY)); }

int RLC(uint8_t* r)
{
  uint8_t msb = *r & 0x80;
  set_flag(&p, CARRY, msb);
  *r <<= 1;

  if (msb)
    *r |= 0x01; // Set bit 0

  set_flag(&p, ZERO, !(*r));
  set_flag(&p, SUBTRACT, 0);
  set_flag(&p, HALF_CARRY, 0);

  p.pc += 2;
  return 8;
}

int RLC_B() { return RLC(&p.bc.r8.high); }
int RLC_C() { return RLC(&p.bc.r8.low); }
int RLC_D() { return RLC(&p.de.r8.high); }
int RLC_E() { return RLC(&p.de.r8.low); }
int RLC_H() { return RLC(&p.hl.r8.high); }
int RLC_L() { return RLC( &p.hl.r8.low); }
int RLC_HL() { return RLC(get_mem_ref(p.hl.r16)); }
int RLC_A() { return RLC(&p.af.r8.high); }

int RRC(uint8_t* r)
{
  uint8_t lsb = *r & 0x01;
  set_flag(&p, CARRY, lsb);
  *r >>= 1;

  if (lsb)
    *r |= 0x80; // Set bit 7

  set_flag(&p, ZERO, !(*r));
  set_flag(&p, SUBTRACT, 0);
  set_flag(&p, HALF_CARRY, 0);

  p.pc += 2;
  return 8;
}

int RRC_B() { return RRC(&p.bc.r8.high); }
int RRC_C() { return RRC(&p.bc.r8.low); }
int RRC_D() { return RRC(&p.de.r8.high); }
int RRC_E() { return RRC(&p.de.r8.low); }
int RRC_H() { return RRC(&p.hl.r8.high); }
int RRC_L() { return RRC(&p.hl.r8.low); }
int RRC_HL() { return RRC(get_mem_ref(p.hl.r16)); }
int RRC_A() { return RRC(&p.af.r8.high); }

int RL(uint8_t* r)
{
  uint8_t msb = *r & 0x80;
  uint8_t old_carry = test_flag(&p, CARRY);
  set_flag(&p, CARRY, msb);
  *r <<= 1;

  if (old_carry)
    *r |= 0x01; // Set bit 0

  set_flag(&p, ZERO, !(*r));
  set_flag(&p, SUBTRACT, 0);
  set_flag(&p, HALF_CARRY, 0);

  p.pc += 2;
  return 8;
}

int RL_B() { return RL(&p.bc.r8.high); }
int RL_C() { return RL(&p.bc.r8.low); }
int RL_D() { return RL(&p.de.r8.high); }
int RL_E() { return RL(&p.de.r8.low); }
int RL_H() { return RL(&p.hl.r8.high); }
int RL_L() { return RL(&p.hl.r8.low); }
int RL_HL() { return RL(get_mem_ref(p.hl.r16)); }
int RL_A() { return RL(&p.af.r8.high); }

int RR(uint8_t* r)
{
  uint8_t lsb = (*r) & 0x01;
  uint8_t old_carry = test_flag(&p, CARRY);
  set_flag(&p, CARRY, lsb);
  *r >>= 1;

  if (old_carry)
    *r |= 0x80; // Set bit 7

  set_flag(&p, ZERO, !(*r));
  set_flag(&p, SUBTRACT, 0);
  set_flag(&p, HALF_CARRY, 0);

  p.pc += 2;
  return 8;
}

int RR_B() { return RR(&p.bc.r8.high); }
int RR_C() { return RR(&p.bc.r8.low); }
int RR_D() { return RR(&p.de.r8.high); }
int RR_E() { return RR(&p.de.r8.low); }
int RR_H() { return RR(&p.hl.r8.high); }
int RR_L() { return RR(&p.hl.r8.low); }
int RR_HL() { return RR(get_mem_ref(p.hl.r16)); }
int RR_A() { return RR(&p.af.r8.high); }

int RLCA()
{
  RLC(&p.af.r8.high);
  set_flag(&p, ZERO, 0);
  p.pc--; // RLCA is a 1 byte instruction, but RLC increments pc by 2
  return 4;
}

int RLA()
{
  RL(&p.af.r8.high);
  set_flag(&p, ZERO, 0);
  p.pc--; // RLA is a 1 byte instruction, but RL increments pc by 2
  return 4;
}

int RRCA()
{
  RRC(&p.af.r8.high);
  set_flag(&p, ZERO, 0);
  p.pc--; // RRCA is a 1 byte instruction, but RRC increments pc by 2
  return 4;
}

int RRA()
{
  RR(&p.af.r8.high);
  set_flag(&p, ZERO, 0);
  p.pc--; // RRA is a 1 byte instruction, but RR increments pc by 2
  return 4;
}

int SLA(uint8_t* r)
{
  uint8_t msb = *r & 0x80;
  set_flag(&p, CARRY, msb);
  *r <<= 1;

  set_flag(&p, ZERO, !(*r));
  set_flag(&p, SUBTRACT, 0);
  set_flag(&p, HALF_CARRY, 0);

  p.pc += 2;
  return 8;
}

int SLA_B() { return SLA(&p.bc.r8.high); }
int SLA_C() { return SLA(&p.bc.r8.low); }
int SLA_D() { return SLA(&p.de.r8.high); }
int SLA_E() { return SLA(&p.de.r8.low); }
int SLA_H() { return SLA(&p.hl.r8.high); }
int SLA_L() { return SLA(&p.hl.r8.low); }
int SLA_HL() { return SLA(get_mem_ref(p.hl.r16)); }
int SLA_A() { return SLA(&p.af.r8.high); }

int SRA(uint8_t* r)
{
  uint8_t lsb = *r & 0x01;
  uint8_t msb = *r & 0x80;
  set_flag(&p, CARRY, lsb);
  *r >>= 1;

  if (msb)
    *r |= 0x80;

  set_flag(&p, ZERO, !(*r));
  set_flag(&p, SUBTRACT, 0);
  set_flag(&p, HALF_CARRY, 0);

  p.pc += 2;
  return 8;
}

int SRA_B() { return SRA(&p.bc.r8.high); }
int SRA_C() { return SRA(&p.bc.r8.low); }
int SRA_D() { return SRA(&p.de.r8.high); }
int SRA_E() { return SRA(&p.de.r8.low); }
int SRA_H() { return SRA(&p.hl.r8.high); }
int SRA_L() { return SRA(&p.hl.r8.low); }
int SRA_HL() { return SRA(get_mem_ref(p.hl.r16)); }
int SRA_A() { return SRA(&p.af.r8.high); }

int SRL(uint8_t* r)
{
  uint8_t lsb = *r & 0x01;
  set_flag(&p, CARRY, lsb);
  *r >>= 1;

  set_flag(&p, ZERO, !(*r));
  set_flag(&p, SUBTRACT, 0);
  set_flag(&p, HALF_CARRY, 0);

  p.pc += 2;
  return 8;
}

int SRL_B() { return SRL(&p.bc.r8.high); }
int SRL_C() { return SRL(&p.bc.r8.low); }
int SRL_D() { return SRL(&p.de.r8.high); }
int SRL_E() { return SRL(&p.de.r8.low); }
int SRL_H() { return SRL(&p.hl.r8.high); }
int SRL_L() { return SRL(&p.hl.r8.low); }
int SRL_HL() { return SRL(get_mem_ref(p.hl.r16)); }
int SRL_A() { return SRL(&p.af.r8.high); }

int SWAP(uint8_t* r)
{
  uint8_t lower_nibble = *r & 0x0F;
  uint8_t upper_nibble = (*r & 0xF0) >> 4;

  *r = (lower_nibble << 4) + upper_nibble;

  p.pc += 2;
  return 8;
}

int SWAP_B() { return SWAP(&p.bc.r8.high); }
int SWAP_C() { return SWAP(&p.bc.r8.low); }
int SWAP_D() { return SWAP(&p.de.r8.high); }
int SWAP_E() { return SWAP(&p.de.r8.low); }
int SWAP_H() { return SWAP(&p.hl.r8.high); }
int SWAP_L() { return SWAP(&p.hl.r8.low); }
int SWAP_HL() { return SWAP(get_mem_ref(p.hl.r16)); }
int SWAP_A() { return SWAP(&p.af.r8.high); }

int test_bit(uint8_t r, uint8_t bit)
{
  uint8_t bitmask = 1 << bit;
  set_flag(&p, ZERO, !(r & bitmask));
  clear_flags(&p, SUBTRACT);
  set_flag(&p, HALF_CARRY, 1);

  p.pc += 2;
  return 8;
}

int test_bit_0_B() { return test_bit(p.bc.r8.high, 0); }
int test_bit_1_B() { return test_bit(p.bc.r8.high, 1); }
int test_bit_2_B() { return test_bit(p.bc.r8.high, 2); }
int test_bit_3_B() { return test_bit(p.bc.r8.high, 3); }
int test_bit_4_B() { return test_bit(p.bc.r8.high, 4); }
int test_bit_5_B() { return test_bit(p.bc.r8.high, 5); }
int test_bit_6_B() { return test_bit(p.bc.r8.high, 6); }
int test_bit_7_B() { return test_bit(p.bc.r8.high, 7); }

int test_bit_0_C() { return test_bit(p.bc.r8.low, 0); }
int test_bit_1_C() { return test_bit(p.bc.r8.low, 1); }
int test_bit_2_C() { return test_bit(p.bc.r8.low, 2); }
int test_bit_3_C() { return test_bit(p.bc.r8.low, 3); }
int test_bit_4_C() { return test_bit(p.bc.r8.low, 4); }
int test_bit_5_C() { return test_bit(p.bc.r8.low, 5); }
int test_bit_6_C() { return test_bit(p.bc.r8.low, 6); }
int test_bit_7_C() { return test_bit(p.bc.r8.low, 7); }

int test_bit_0_D() { return test_bit(p.de.r8.high, 0); }
int test_bit_1_D() { return test_bit(p.de.r8.high, 1); }
int test_bit_2_D() { return test_bit(p.de.r8.high, 2); }
int test_bit_3_D() { return test_bit(p.de.r8.high, 3); }
int test_bit_4_D() { return test_bit(p.de.r8.high, 4); }
int test_bit_5_D() { return test_bit(p.de.r8.high, 5); }
int test_bit_6_D() { return test_bit(p.de.r8.high, 6); }
int test_bit_7_D() { return test_bit(p.de.r8.high, 7); }

int test_bit_0_E() { return test_bit(p.de.r8.low, 0); }
int test_bit_1_E() { return test_bit(p.de.r8.low, 1); }
int test_bit_2_E() { return test_bit(p.de.r8.low, 2); }
int test_bit_3_E() { return test_bit(p.de.r8.low, 3); }
int test_bit_4_E() { return test_bit(p.de.r8.low, 4); }
int test_bit_5_E() { return test_bit(p.de.r8.low, 5); }
int test_bit_6_E() { return test_bit(p.de.r8.low, 6); }
int test_bit_7_E() { return test_bit(p.de.r8.low, 7); }

int test_bit_0_A() { return test_bit(p.af.r8.high, 0); }
int test_bit_1_A() { return test_bit(p.af.r8.high, 1); }
int test_bit_2_A() { return test_bit(p.af.r8.high, 2); }
int test_bit_3_A() { return test_bit(p.af.r8.high, 3); }
int test_bit_4_A() { return test_bit(p.af.r8.high, 4); }
int test_bit_5_A() { return test_bit(p.af.r8.high, 5); }
int test_bit_6_A() { return test_bit(p.af.r8.high, 6); }
int test_bit_7_A() { return test_bit(p.af.r8.high, 7); }

int test_bit_0_H() { return test_bit(p.hl.r8.high, 0); }
int test_bit_1_H() { return test_bit(p.hl.r8.high, 1); }
int test_bit_2_H() { return test_bit(p.hl.r8.high, 2); }
int test_bit_3_H() { return test_bit(p.hl.r8.high, 3); }
int test_bit_4_H() { return test_bit(p.hl.r8.high, 4); }
int test_bit_5_H() { return test_bit(p.hl.r8.high, 5); }
int test_bit_6_H() { return test_bit(p.hl.r8.high, 6); }
int test_bit_7_H() { return test_bit(p.hl.r8.high, 7); }

int test_bit_0_L() { return test_bit(p.hl.r8.low, 0); }
int test_bit_1_L() { return test_bit(p.hl.r8.low, 1); }
int test_bit_2_L() { return test_bit(p.hl.r8.low, 2); }
int test_bit_3_L() { return test_bit(p.hl.r8.low, 3); }
int test_bit_4_L() { return test_bit(p.hl.r8.low, 4); }
int test_bit_5_L() { return test_bit(p.hl.r8.low, 5); }
int test_bit_6_L() { return test_bit(p.hl.r8.low, 6); }
int test_bit_7_L() { return test_bit(p.hl.r8.low, 7); }

int test_bit_0_HL() { return test_bit(read_from_mem(p.hl.r16), 0); }
int test_bit_1_HL() { return test_bit(read_from_mem(p.hl.r16), 1); }
int test_bit_2_HL() { return test_bit(read_from_mem(p.hl.r16), 2); }
int test_bit_3_HL() { return test_bit(read_from_mem(p.hl.r16), 3); }
int test_bit_4_HL() { return test_bit(read_from_mem(p.hl.r16), 4); }
int test_bit_5_HL() { return test_bit(read_from_mem(p.hl.r16), 5); }
int test_bit_6_HL() { return test_bit(read_from_mem(p.hl.r16), 6); }
int test_bit_7_HL() { return test_bit(read_from_mem(p.hl.r16), 7); }

int set_bit(uint8_t* r, uint8_t bit)
{
  uint8_t bitmask = 1 << bit;
  *r |= bitmask;

  p.pc += 2;
  return 8;
}

int set_bit_0_B() { return set_bit(&p.bc.r8.high, 0); }
int set_bit_1_B() { return set_bit(&p.bc.r8.high, 1); }
int set_bit_2_B() { return set_bit(&p.bc.r8.high, 2); }
int set_bit_3_B() { return set_bit(&p.bc.r8.high, 3); }
int set_bit_4_B() { return set_bit(&p.bc.r8.high, 4); }
int set_bit_5_B() { return set_bit(&p.bc.r8.high, 5); }
int set_bit_6_B() { return set_bit(&p.bc.r8.high, 6); }
int set_bit_7_B() { return set_bit(&p.bc.r8.high, 7); }

int set_bit_0_C() { return set_bit(&p.bc.r8.low, 0); }
int set_bit_1_C() { return set_bit(&p.bc.r8.low, 1); }
int set_bit_2_C() { return set_bit(&p.bc.r8.low, 2); }
int set_bit_3_C() { return set_bit(&p.bc.r8.low, 3); }
int set_bit_4_C() { return set_bit(&p.bc.r8.low, 4); }
int set_bit_5_C() { return set_bit(&p.bc.r8.low, 5); }
int set_bit_6_C() { return set_bit(&p.bc.r8.low, 6); }
int set_bit_7_C() { return set_bit(&p.bc.r8.low, 7); }

int set_bit_0_D() { return set_bit(&p.de.r8.high, 0); }
int set_bit_1_D() { return set_bit(&p.de.r8.high, 1); }
int set_bit_2_D() { return set_bit(&p.de.r8.high, 2); }
int set_bit_3_D() { return set_bit(&p.de.r8.high, 3); }
int set_bit_4_D() { return set_bit(&p.de.r8.high, 4); }
int set_bit_5_D() { return set_bit(&p.de.r8.high, 5); }
int set_bit_6_D() { return set_bit(&p.de.r8.high, 6); }
int set_bit_7_D() { return set_bit(&p.de.r8.high, 7); }

int set_bit_0_E() { return set_bit(&p.de.r8.low, 0); }
int set_bit_1_E() { return set_bit(&p.de.r8.low, 1); }
int set_bit_2_E() { return set_bit(&p.de.r8.low, 2); }
int set_bit_3_E() { return set_bit(&p.de.r8.low, 3); }
int set_bit_4_E() { return set_bit(&p.de.r8.low, 4); }
int set_bit_5_E() { return set_bit(&p.de.r8.low, 5); }
int set_bit_6_E() { return set_bit(&p.de.r8.low, 6); }
int set_bit_7_E() { return set_bit(&p.de.r8.low, 7); }

int set_bit_0_A() { return set_bit(&p.af.r8.high, 0); }
int set_bit_1_A() { return set_bit(&p.af.r8.high, 1); }
int set_bit_2_A() { return set_bit(&p.af.r8.high, 2); }
int set_bit_3_A() { return set_bit(&p.af.r8.high, 3); }
int set_bit_4_A() { return set_bit(&p.af.r8.high, 4); }
int set_bit_5_A() { return set_bit(&p.af.r8.high, 5); }
int set_bit_6_A() { return set_bit(&p.af.r8.high, 6); }
int set_bit_7_A() { return set_bit(&p.af.r8.high, 7); }

int set_bit_0_H() { return set_bit(&p.hl.r8.high, 0); }
int set_bit_1_H() { return set_bit(&p.hl.r8.high, 1); }
int set_bit_2_H() { return set_bit(&p.hl.r8.high, 2); }
int set_bit_3_H() { return set_bit(&p.hl.r8.high, 3); }
int set_bit_4_H() { return set_bit(&p.hl.r8.high, 4); }
int set_bit_5_H() { return set_bit(&p.hl.r8.high, 5); }
int set_bit_6_H() { return set_bit(&p.hl.r8.high, 6); }
int set_bit_7_H() { return set_bit(&p.hl.r8.high, 7); }

int set_bit_0_L() { return set_bit(&p.hl.r8.low, 0); }
int set_bit_1_L() { return set_bit(&p.hl.r8.low, 1); }
int set_bit_2_L() { return set_bit(&p.hl.r8.low, 2); }
int set_bit_3_L() { return set_bit(&p.hl.r8.low, 3); }
int set_bit_4_L() { return set_bit(&p.hl.r8.low, 4); }
int set_bit_5_L() { return set_bit(&p.hl.r8.low, 5); }
int set_bit_6_L() { return set_bit(&p.hl.r8.low, 6); }
int set_bit_7_L() { return set_bit(&p.hl.r8.low, 7); }

int set_bit_0_HL() { return set_bit(get_mem_ref(p.hl.r16), 0); }
int set_bit_1_HL() { return set_bit(get_mem_ref(p.hl.r16), 1); }
int set_bit_2_HL() { return set_bit(get_mem_ref(p.hl.r16), 2); }
int set_bit_3_HL() { return set_bit(get_mem_ref(p.hl.r16), 3); }
int set_bit_4_HL() { return set_bit(get_mem_ref(p.hl.r16), 4); }
int set_bit_5_HL() { return set_bit(get_mem_ref(p.hl.r16), 5); }
int set_bit_6_HL() { return set_bit(get_mem_ref(p.hl.r16), 6); }
int set_bit_7_HL() { return set_bit(get_mem_ref(p.hl.r16), 7); }

int reset_bit(uint8_t* r, uint8_t bit)
{
  uint8_t bitmask = 1 << bit;
  *r &= ~bitmask;

  p.pc += 2;
  return 8;
}

int reset_bit_0_B() { return reset_bit(&p.bc.r8.high, 0); }
int reset_bit_1_B() { return reset_bit(&p.bc.r8.high, 1); }
int reset_bit_2_B() { return reset_bit(&p.bc.r8.high, 2); }
int reset_bit_3_B() { return reset_bit(&p.bc.r8.high, 3); }
int reset_bit_4_B() { return reset_bit(&p.bc.r8.high, 4); }
int reset_bit_5_B() { return reset_bit(&p.bc.r8.high, 5); }
int reset_bit_6_B() { return reset_bit(&p.bc.r8.high, 6); }
int reset_bit_7_B() { return reset_bit(&p.bc.r8.high, 7); }

int reset_bit_0_C() { return reset_bit(&p.bc.r8.low, 0); }
int reset_bit_1_C() { return reset_bit(&p.bc.r8.low, 1); }
int reset_bit_2_C() { return reset_bit(&p.bc.r8.low, 2); }
int reset_bit_3_C() { return reset_bit(&p.bc.r8.low, 3); }
int reset_bit_4_C() { return reset_bit(&p.bc.r8.low, 4); }
int reset_bit_5_C() { return reset_bit(&p.bc.r8.low, 5); }
int reset_bit_6_C() { return reset_bit(&p.bc.r8.low, 6); }
int reset_bit_7_C() { return reset_bit(&p.bc.r8.low, 7); }

int reset_bit_0_D() { return reset_bit(&p.de.r8.high, 0); }
int reset_bit_1_D() { return reset_bit(&p.de.r8.high, 1); }
int reset_bit_2_D() { return reset_bit(&p.de.r8.high, 2); }
int reset_bit_3_D() { return reset_bit(&p.de.r8.high, 3); }
int reset_bit_4_D() { return reset_bit(&p.de.r8.high, 4); }
int reset_bit_5_D() { return reset_bit(&p.de.r8.high, 5); }
int reset_bit_6_D() { return reset_bit(&p.de.r8.high, 6); }
int reset_bit_7_D() { return reset_bit(&p.de.r8.high, 7); }

int reset_bit_0_E() { return reset_bit(&p.de.r8.low, 0); }
int reset_bit_1_E() { return reset_bit(&p.de.r8.low, 1); }
int reset_bit_2_E() { return reset_bit(&p.de.r8.low, 2); }
int reset_bit_3_E() { return reset_bit(&p.de.r8.low, 3); }
int reset_bit_4_E() { return reset_bit(&p.de.r8.low, 4); }
int reset_bit_5_E() { return reset_bit(&p.de.r8.low, 5); }
int reset_bit_6_E() { return reset_bit(&p.de.r8.low, 6); }
int reset_bit_7_E() { return reset_bit(&p.de.r8.low, 7); }

int reset_bit_0_A() { return reset_bit(&p.af.r8.high, 0); }
int reset_bit_1_A() { return reset_bit(&p.af.r8.high, 1); }
int reset_bit_2_A() { return reset_bit(&p.af.r8.high, 2); }
int reset_bit_3_A() { return reset_bit(&p.af.r8.high, 3); }
int reset_bit_4_A() { return reset_bit(&p.af.r8.high, 4); }
int reset_bit_5_A() { return reset_bit(&p.af.r8.high, 5); }
int reset_bit_6_A() { return reset_bit(&p.af.r8.high, 6); }
int reset_bit_7_A() { return reset_bit(&p.af.r8.high, 7); }

int reset_bit_0_H() { return reset_bit(&p.hl.r8.high, 0); }
int reset_bit_1_H() { return reset_bit(&p.hl.r8.high, 1); }
int reset_bit_2_H() { return reset_bit(&p.hl.r8.high, 2); }
int reset_bit_3_H() { return reset_bit(&p.hl.r8.high, 3); }
int reset_bit_4_H() { return reset_bit(&p.hl.r8.high, 4); }
int reset_bit_5_H() { return reset_bit(&p.hl.r8.high, 5); }
int reset_bit_6_H() { return reset_bit(&p.hl.r8.high, 6); }
int reset_bit_7_H() { return reset_bit(&p.hl.r8.high, 7); }

int reset_bit_0_L() { return reset_bit(&p.hl.r8.low, 0); }
int reset_bit_1_L() { return reset_bit(&p.hl.r8.low, 1); }
int reset_bit_2_L() { return reset_bit(&p.hl.r8.low, 2); }
int reset_bit_3_L() { return reset_bit(&p.hl.r8.low, 3); }
int reset_bit_4_L() { return reset_bit(&p.hl.r8.low, 4); }
int reset_bit_5_L() { return reset_bit(&p.hl.r8.low, 5); }
int reset_bit_6_L() { return reset_bit(&p.hl.r8.low, 6); }
int reset_bit_7_L() { return reset_bit(&p.hl.r8.low, 7); }

int reset_bit_0_HL() { return reset_bit(get_mem_ref(p.hl.r16), 0); }
int reset_bit_1_HL() { return reset_bit(get_mem_ref(p.hl.r16), 1); }
int reset_bit_2_HL() { return reset_bit(get_mem_ref(p.hl.r16), 2); }
int reset_bit_3_HL() { return reset_bit(get_mem_ref(p.hl.r16), 3); }
int reset_bit_4_HL() { return reset_bit(get_mem_ref(p.hl.r16), 4); }
int reset_bit_5_HL() { return reset_bit(get_mem_ref(p.hl.r16), 5); }
int reset_bit_6_HL() { return reset_bit(get_mem_ref(p.hl.r16), 6); }
int reset_bit_7_HL() { return reset_bit(get_mem_ref(p.hl.r16), 7); }

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

int DI()
{
  p.interrupts_enabled = 0;

  p.pc++;
  return 4;
}

int EI()
{
  p.interrupts_enabled = 1;

  p.pc++;
  return 4;
}

int INC_r8(uint8_t* r)
{
  set_flag(&p, HALF_CARRY, calculate_half_carry(*r, 1));

  (*r)++;

  if (*r == 0)
    set_flag(&p, ZERO, 1);
  else
    clear_flags(&p, ZERO);
  clear_flags(&p, SUBTRACT);

  p.pc++;
  return 4;
}

int INC_B() { return INC_r8(&p.bc.r8.high); }
int INC_D() { return INC_r8(&p.de.r8.high); }
int INC_H() { return INC_r8(&p.hl.r8.high); }
int INC_mHL() { return INC_r8(get_mem_ref(p.hl.r16)); }

int INC_C() { return INC_r8(&p.bc.r8.low); }
int INC_E() { return INC_r8(&p.de.r8.low); }
int INC_L() { return INC_r8(&p.hl.r8.low); }
int INC_A() { return INC_r8(&p.af.r8.high); }

int DEC_r8(uint8_t* r)
{
  set_flag(&p, HALF_CARRY, (*r & 0x0F) < 1);

  (*r)--;

  if (*r == 0)
    set_flag(&p, ZERO, 1);
  else
    clear_flags(&p, ZERO);
  set_flag(&p, SUBTRACT, 1);

  p.pc++;
  return 4;
}

int DEC_B() { return DEC_r8(&p.bc.r8.high); }
int DEC_D() { return DEC_r8(&p.de.r8.high); }
int DEC_H() { return DEC_r8(&p.hl.r8.high); }
int DEC_mHL() { return DEC_r8(get_mem_ref(p.hl.r16)); }

int DEC_C() { return DEC_r8(&p.bc.r8.low); }
int DEC_E() { return DEC_r8(&p.de.r8.low); }
int DEC_L() { return DEC_r8(&p.hl.r8.low); }
int DEC_A() { return DEC_r8(&p.af.r8.high); }

int LD_mHL_A_inc()
{
  write_to_mem(p.hl.r16++, p.af.r8.high);

  p.pc++;
  return 8;
}

int LD_mHL_A_dec()
{
  write_to_mem(p.hl.r16--, p.af.r8.high);

  p.pc++;
  return 8;
}

int LD_A_mHL_inc()
{
  p.af.r8.high = read_from_mem(p.hl.r16++);

  p.pc++;
  return 8;
}

int LD_A_mHL_dec()
{
  p.af.r8.high = read_from_mem(p.hl.r16--);

  p.pc++;
  return 8;
}

int LD_A_a16()
{
  uint16_t addr = generate_address(read_from_mem(p.pc+1), read_from_mem(p.pc+2));
  p.af.r8.high = read_from_mem(addr);

  p.pc += 3;
  return 16;
}

int LD_mC_A()
{
  uint16_t addr = generate_address(p.bc.r8.low, 0xFF);
  write_to_mem(addr, p.af.r8.high);

  p.pc += 2;
  return 8;
}

int LD_A_mC()
{
  uint16_t addr = generate_address(p.bc.r8.low, 0xFF);
  p.af.r8.high = read_from_mem(addr);

  p.pc += 2;
  return 8;
}

int LD_a16_A()
{
  uint16_t addr = generate_address(read_from_mem(p.pc+1), read_from_mem(p.pc+2));
  write_to_mem(addr, p.af.r8.high);

  p.pc += 3;
  return 16;
}

int INC_r16(uint16_t* r)
{
  (*r)++;

  p.pc++;
  return 8;
}

int INC_BC() { return INC_r16(&p.bc.r16); }
int INC_DE() { return INC_r16(&p.de.r16); }
int INC_HL() { return INC_r16(&p.hl.r16); }
int INC_SP() { return INC_r16(&p.sp); }

int DEC(uint16_t* r)
{
  (*r)--;

  p.pc++;
  return 8;
}

int DEC_BC() { return DEC(&p.bc.r16); }
int DEC_DE() { return DEC(&p.de.r16); }
int DEC_HL() { return DEC(&p.hl.r16); }
int DEC_SP() { return DEC(&p.sp); }

int LD_d8(uint8_t* nn)
{
  *nn = read_from_mem(p.pc+1);

  p.pc += 2;
  return 8;
}

int LD_B() { return LD_d8(&p.bc.r8.high); }
int LD_D() { return LD_d8(&p.de.r8.high); }
int LD_H() { return LD_d8(&p.hl.r8.high); }
int LD_mHL() { return LD_d8(get_mem_ref(p.hl.r16)); }

int LD_C() { return LD_d8(&p.bc.r8.low); }
int LD_E() { return LD_d8(&p.de.r8.low); }
int LD_L() { return LD_d8(&p.hl.r8.low); }
int LD_A() { return LD_d8(&p.af.r8.high); }

int LD_d16(reg* r)
{
  r->r8.low = read_from_mem(p.pc+1);
  r->r8.high = read_from_mem(p.pc+2);

  p.pc += 3;
  return 12;
}

int LD_BC() { return LD_d16(&p.bc); }
int LD_DE() { return LD_d16(&p.de); }
int LD_HL() { return LD_d16(&p.hl); }
int LD_SP()
{
  uint8_t low = read_from_mem(p.pc+1);
  uint8_t high = read_from_mem(p.pc+2);

  p.sp = generate_address(low, high);

  p.pc += 3;
  return 12;
}

int LDHL_SP_r8()
{
  uint8_t d8 = read_from_mem(p.pc+1);
  uint16_t res = p.sp + d8;

  uint8_t is_negative = (d8 & 0x80);
  if (is_negative)
  {
    set_flag(&p, HALF_CARRY, (res & 0x0F) <= (p.sp & 0x0F));
    set_flag(&p, CARRY, (res & 0xFF) <= (p.sp & 0xFF));
  }
  else
  {
    int half_sum = (p.sp & 0x0F) + (d8 & 0x0F);
    set_flag(&p, HALF_CARRY, half_sum >= 0x100);
    int sum = (p.sp & 0xFF) + d8;
    set_flag(&p, CARRY, sum >= 0x10000);
  }

  clear_flags(&p, ZERO | SUBTRACT);

  p.hl.r16 = res;

  p.pc += 2;
  return 12;
}

int LD_reg_mem(uint8_t* reg, uint8_t* mem)
{
  *reg = *mem;

  p.pc++;
  return 8;
}

int LD_mBC_A() { return LD_reg_mem(get_mem_ref(p.bc.r16), &p.af.r8.high); }
int LD_mDE_A() { return LD_reg_mem(get_mem_ref(p.de.r16), &p.af.r8.high); }
int LD_A_mBC() { return LD_reg_mem(&p.af.r8.high, get_mem_ref(p.bc.r16)); }
int LD_A_mDE() { return LD_reg_mem(&p.af.r8.high, get_mem_ref(p.de.r16)); }
int LD_B_mHL() { return LD_reg_mem(&p.bc.r8.high, get_mem_ref(p.hl.r16)); }
int LD_D_mHL() { return LD_reg_mem(&p.de.r8.high, get_mem_ref(p.hl.r16)); }
int LD_H_mHL() { return LD_reg_mem(&p.hl.r8.high, get_mem_ref(p.hl.r16)); }
int LD_C_mHL() { return LD_reg_mem(&p.bc.r8.low, get_mem_ref(p.hl.r16)); }
int LD_E_mHL() { return LD_reg_mem(&p.de.r8.low, get_mem_ref(p.hl.r16)); }
int LD_L_mHL() { return LD_reg_mem(&p.hl.r8.low, get_mem_ref(p.hl.r16)); }
int LD_A_mHL() { return LD_reg_mem(&p.af.r8.low, get_mem_ref(p.hl.r16)); }
int LD_mHL_B() { return LD_reg_mem(get_mem_ref(p.hl.r16), &p.bc.r8.high); }
int LD_mHL_C() { return LD_reg_mem(get_mem_ref(p.hl.r16), &p.bc.r8.low); }
int LD_mHL_D() { return LD_reg_mem(get_mem_ref(p.hl.r16), &p.de.r8.high); }
int LD_mHL_E() { return LD_reg_mem(get_mem_ref(p.hl.r16), &p.de.r8.low); }
int LD_mHL_H() { return LD_reg_mem(get_mem_ref(p.hl.r16), &p.hl.r8.high); }
int LD_mHL_L() { return LD_reg_mem(get_mem_ref(p.hl.r16), &p.hl.r8.low); }
int LD_mHL_A() { return LD_reg_mem(get_mem_ref(p.hl.r16), &p.af.r8.high); }

int LD_reg(uint8_t* r1, uint8_t* r2)
{
  *r1 = *r2;

  p.pc++;
  return 4;
}

int LD_a16_SP()
{
  uint8_t low_byte = read_from_mem(p.pc+1);
  uint8_t high_byte = read_from_mem(p.pc+2);
  uint16_t addr = generate_address(low_byte, high_byte);

  write_to_mem(addr, p.sp);

  p.pc += 3;
  return 20;
}

int LD_B_B() { return LD_reg(&p.bc.r8.high, &p.bc.r8.high); }
int LD_B_C() { return LD_reg(&p.bc.r8.high, &p.bc.r8.low); }
int LD_B_D() { return LD_reg(&p.bc.r8.high, &p.de.r8.high); }
int LD_B_E() { return LD_reg(&p.bc.r8.high, &p.de.r8.low); }
int LD_B_H() { return LD_reg(&p.bc.r8.high, &p.hl.r8.high); }
int LD_B_L() { return LD_reg(&p.bc.r8.high, &p.hl.r8.low); }
int LD_B_A() { return LD_reg(&p.bc.r8.high, &p.af.r8.high); }

int LD_C_B() { return LD_reg(&p.bc.r8.low, &p.bc.r8.high); }
int LD_C_C() { return LD_reg(&p.bc.r8.low, &p.bc.r8.low); }
int LD_C_D() { return LD_reg(&p.bc.r8.low, &p.de.r8.high); }
int LD_C_E() { return LD_reg(&p.bc.r8.low, &p.de.r8.low); }
int LD_C_H() { return LD_reg(&p.bc.r8.low, &p.hl.r8.high); }
int LD_C_L() { return LD_reg(&p.bc.r8.low, &p.hl.r8.low); }
int LD_C_A() { return LD_reg(&p.bc.r8.low, &p.af.r8.high); }

int LD_D_B() { return LD_reg(&p.de.r8.high, &p.bc.r8.high); }
int LD_D_C() { return LD_reg(&p.de.r8.high, &p.bc.r8.low); }
int LD_D_D() { return LD_reg(&p.de.r8.high, &p.de.r8.high); }
int LD_D_E() { return LD_reg(&p.de.r8.high, &p.de.r8.low); }
int LD_D_H() { return LD_reg(&p.de.r8.high, &p.hl.r8.high); }
int LD_D_L() { return LD_reg(&p.de.r8.high, &p.hl.r8.low); }
int LD_D_A() { return LD_reg(&p.de.r8.high, &p.af.r8.high); }

int LD_E_B() { return LD_reg(&p.de.r8.low, &p.bc.r8.high); }
int LD_E_C() { return LD_reg(&p.de.r8.low, &p.bc.r8.low); }
int LD_E_D() { return LD_reg(&p.de.r8.low, &p.de.r8.high); }
int LD_E_E() { return LD_reg(&p.de.r8.low, &p.de.r8.low); }
int LD_E_H() { return LD_reg(&p.de.r8.low, &p.hl.r8.high); }
int LD_E_L() { return LD_reg(&p.de.r8.low, &p.hl.r8.low); }
int LD_E_A() { return LD_reg(&p.de.r8.low, &p.af.r8.high); }

int LD_H_B() { return LD_reg(&p.hl.r8.high, &p.bc.r8.high); }
int LD_H_C() { return LD_reg(&p.hl.r8.high, &p.bc.r8.low); }
int LD_H_D() { return LD_reg(&p.hl.r8.high, &p.de.r8.high); }
int LD_H_E() { return LD_reg(&p.hl.r8.high, &p.de.r8.low); }
int LD_H_H() { return LD_reg(&p.hl.r8.high, &p.hl.r8.high); }
int LD_H_L() { return LD_reg(&p.hl.r8.high, &p.hl.r8.low); }
int LD_H_A() { return LD_reg(&p.hl.r8.high, &p.af.r8.high); }

int LD_L_B() { return LD_reg(&p.hl.r8.low, &p.bc.r8.high); }
int LD_L_C() { return LD_reg(&p.hl.r8.low, &p.bc.r8.low); }
int LD_L_D() { return LD_reg(&p.hl.r8.low, &p.de.r8.high); }
int LD_L_E() { return LD_reg(&p.hl.r8.low, &p.de.r8.low); }
int LD_L_H() { return LD_reg(&p.hl.r8.low, &p.hl.r8.high); }
int LD_L_L() { return LD_reg(&p.hl.r8.low, &p.hl.r8.low); }
int LD_L_A() { return LD_reg(&p.hl.r8.low, &p.af.r8.high); }

int LD_A_B() { return LD_reg(&p.af.r8.high, &p.bc.r8.high); }
int LD_A_C() { return LD_reg(&p.af.r8.high, &p.bc.r8.low); }
int LD_A_D() { return LD_reg(&p.af.r8.high, &p.de.r8.high); }
int LD_A_E() { return LD_reg(&p.af.r8.high, &p.de.r8.low); }
int LD_A_H() { return LD_reg(&p.af.r8.high, &p.hl.r8.high); }
int LD_A_L() { return LD_reg(&p.af.r8.high, &p.hl.r8.low); }
int LD_A_A() { return LD_reg(&p.af.r8.high, &p.af.r8.high); }

int NOP() { p.pc++; return 4; }

int JP()
{
  uint16_t adr = generate_address(read_from_mem(p.pc+1), read_from_mem(p.pc+2));
  p.pc = adr;
  return 16;
}

int COND_JP(uint8_t cond)
{
  if (cond)
  {
    uint16_t adr = generate_address(read_from_mem(p.pc+1), read_from_mem(p.pc+2));
    p.pc = adr;
    return 16;
  }

  p.pc += 3;
  return 12;
}

int JP_NZ() { return COND_JP(!test_flag(&p, ZERO)); }
int JP_Z() { return COND_JP(test_flag(&p, ZERO)); }
int JP_NC() { return COND_JP(!test_flag(&p, CARRY)); }
int JP_C() { return COND_JP(test_flag(&p, CARRY)); }

int JP_HL()
{
  p.pc = p.hl.r16;
  return 4;
}

int JR()
{
  uint16_t adr = p.pc + 2 + (int8_t) read_from_mem(p.pc+1);
  p.pc = adr;
  return 12;
}

int COND_JR(uint8_t cond)
{
  if (cond)
  {
    uint16_t adr = p.pc + 2 + (int8_t) read_from_mem(p.pc+1);
    p.pc = adr;
    return 12;
  }

  p.pc += 2;
  return 8;
}

int JR_NZ() { return COND_JR(!test_flag(&p, ZERO)); }
int JR_Z() { return COND_JR(test_flag(&p, ZERO)); }
int JR_NC() { return COND_JR(!test_flag(&p, CARRY)); }
int JR_C() { return COND_JR(test_flag(&p, CARRY)); }

int XOR(uint8_t reg)
{
  p.af.r8.high ^= reg;

  // set flags
  clear_flags(&p, SUBTRACT | HALF_CARRY | CARRY);
  set_flag(&p, ZERO, !p.af.r8.high);

  p.pc++;
  return 4;
}

int XOR_B() { return XOR(p.bc.r8.high); }
int XOR_C() { return XOR(p.bc.r8.low); }
int XOR_D() { return XOR(p.de.r8.high); }
int XOR_E() { return XOR(p.de.r8.low); }
int XOR_H() { return XOR(p.hl.r8.high); }
int XOR_L() { return XOR(p.hl.r8.low); }
int XOR_HL() { return XOR(read_from_mem(p.hl.r16)); }
int XOR_A() { return XOR(p.af.r8.high); }
int XOR_A_d8()
{
  XOR(read_from_mem(p.pc+1));

  p.pc++; // Already incremented once in call to XOR
  return 8;
}

int POP(uint16_t* r)
{
  uint16_t val = generate_address(read_from_mem(p.sp), read_from_mem(p.sp+1));
  *r = val;

  p.sp += 2;
  p.pc += 1;
  return 12;
}

int POP_BC() { return POP(&p.bc.r16); }
int POP_DE() { return POP(&p.de.r16); }
int POP_HL() { return POP(&p.hl.r16); }
int POP_AF()
{
  int ret = POP(&p.af.r16);
  p.af.r8.low &= 0xF0; // Lower nibble of flag register should always be zero

  return ret;
}

int PUSH(reg* r)
{
  write_to_mem(p.sp-1, r->r8.high);
  write_to_mem(p.sp-2, r->r8.low);

  p.sp -= 2;
  p.pc += 1;
  return 16;
}

int PUSH_BC() { return PUSH(&p.bc); }
int PUSH_DE() { return PUSH(&p.de); }
int PUSH_HL() { return PUSH(&p.hl); }
int PUSH_AF() { return PUSH(&p.af); }

int RST(uint8_t addr)
{
  write_to_mem(p.sp+1, p.pc);
  p.pc = addr;

  p.sp++;
  return 16;
}

int RST_00() { return RST(0x00); }
int RST_10() { return RST(0x10); }
int RST_20() { return RST(0x20); }
int RST_30() { return RST(0x30); }

int RST_08() { return RST(0x08); }
int RST_18() { return RST(0x18); }
int RST_28() { return RST(0x28); }
int RST_38() { return RST(0x38); }

int ADD_16bit(uint16_t* r1, uint16_t r2)
{
  clear_flags(&p, SUBTRACT);
  int half_sum = (*r1 & 0xFF) + (r2 & 0xFF);
  set_flag(&p, HALF_CARRY, half_sum >= 0x100);
  int sum = *r1 + r2;
  set_flag(&p, CARRY, sum >= 0x10000);

  *r1 = *r1 + r2;

  p.pc++;
  return 8;
}

int ADD_HL_BC() { return ADD_16bit(&p.hl.r16, p.bc.r16); }
int ADD_HL_DE() { return ADD_16bit(&p.hl.r16, p.de.r16); }
int ADD_HL_HL() { return ADD_16bit(&p.hl.r16, p.hl.r16); }
int ADD_HL_SP() { return ADD_16bit(&p.hl.r16, p.sp); }
int ADD_SP_r8()
{
  ADD_16bit(&p.sp, read_from_mem(p.pc+1));
  clear_flags(&p, ZERO);

  p.pc++; // Already incremented once by call to ADD_16bit
  return 16;
}

int ADD(uint8_t* r1, uint8_t r2, int carry)
{

  uint8_t res = *r1 + r2 + carry;
  int carrybits = *r1 ^ r2 ^ res;

  clear_flags(&p, SUBTRACT);
  int half_sum = (*r1 & 0x0F) + (r2 & 0x0F);
  set_flag(&p, HALF_CARRY, half_sum >= 0x10);
  int sum = *r1 + r2;
  set_flag(&p, CARRY, sum >= 0x100);

  *r1 = *r1 + r2;
  set_flag(&p, ZERO, !(*r1));

  p.pc++;
  return 4;
}

int ADD_A_B() { return ADD(&p.af.r8.high, p.bc.r8.high, 0); }
int ADD_A_C() { return ADD(&p.af.r8.high, p.bc.r8.low, 0); }
int ADD_A_D() { return ADD(&p.af.r8.high, p.de.r8.high, 0); }
int ADD_A_E() { return ADD(&p.af.r8.high, p.de.r8.low, 0); }
int ADD_A_H() { return ADD(&p.af.r8.high, p.hl.r8.high, 0); }
int ADD_A_L() { return ADD(&p.af.r8.high, p.hl.r8.low, 0); }
int ADD_A_mHL() { return ADD(&p.af.r8.high, read_from_mem(p.hl.r16), 0); }
int ADD_A_A() { return ADD(&p.af.r8.high, p.af.r8.high, 0); }
int ADD_A_d8()
{
  ADD(&p.af.r8.high, read_from_mem(p.pc+1), 0);

  p.pc++; // Already incremented once in call to ADD
  return 8;
}

int ADC_A_B() { return ADD(&p.af.r8.high, p.bc.r8.high, test_flag(&p, CARRY)); }
int ADC_A_C() { return ADD(&p.af.r8.high, p.bc.r8.low, test_flag(&p, CARRY)); }
int ADC_A_D() { return ADD(&p.af.r8.high, p.de.r8.high, test_flag(&p, CARRY)); }
int ADC_A_E() { return ADD(&p.af.r8.high, p.de.r8.low, test_flag(&p, CARRY)); }
int ADC_A_H() { return ADD(&p.af.r8.high, p.hl.r8.high, test_flag(&p, CARRY)); }
int ADC_A_L() { return ADD(&p.af.r8.high, p.hl.r8.low, test_flag(&p, CARRY)); }
int ADC_A_mHL() { return ADD(&p.af.r8.high, read_from_mem(p.hl.r16), test_flag(&p, CARRY)); }
int ADC_A_A() { return ADD(&p.af.r8.high, p.af.r8.high, test_flag(&p, CARRY)); }
int ADC_A_d8()
{
  ADD(&p.af.r8.high, read_from_mem(p.pc+1), test_flag(&p, CARRY));

  p.pc++; // Already incremented once in call to ADD
  return 8;
}

int SUB(uint8_t* r1, uint8_t r2, int carry)
{
  set_flag(&p, SUBTRACT, 1);
  set_flag(&p, CARRY, r2 + carry > *r1);
  set_flag(&p, HALF_CARRY, (r2 + carry & 0x0F) > (*r1 & 0x0F)  );

  *r1 -= r2 + carry;
  set_flag(&p, ZERO, !(*r1));

  p.pc++;
  return 4;
}

int SUB_B() { return SUB(&p.af.r8.high, p.bc.r8.high, 0); }
int SUB_C() { return SUB(&p.af.r8.high, p.bc.r8.low, 0); }
int SUB_D() { return SUB(&p.af.r8.high, p.de.r8.high, 0); }
int SUB_E() { return SUB(&p.af.r8.high, p.de.r8.low, 0); }
int SUB_H() { return SUB(&p.af.r8.high, p.hl.r8.high, 0); }
int SUB_L() { return SUB(&p.af.r8.high, p.hl.r8.low, 0); }
int SUB_mHL() { return SUB(&p.af.r8.high, read_from_mem(p.hl.r16), 0); }
int SUB_A() { return SUB(&p.af.r8.high, p.af.r8.high, 0); }
int SUB_A_d8()
{
  SUB(&p.af.r8.high, read_from_mem(p.pc+1), 0);

  p.pc++; // Already incremented once in call to SUB
  return 8;
}

int SBC_B() { return SUB(&p.af.r8.high, p.bc.r8.high, test_flag(&p, CARRY)); }
int SBC_C() { return SUB(&p.af.r8.high, p.bc.r8.low, test_flag(&p, CARRY)); }
int SBC_D() { return SUB(&p.af.r8.high, p.de.r8.high, test_flag(&p, CARRY)); }
int SBC_E() { return SUB(&p.af.r8.high, p.de.r8.low, test_flag(&p, CARRY)); }
int SBC_H() { return SUB(&p.af.r8.high, p.hl.r8.high, test_flag(&p, CARRY)); }
int SBC_L() { return SUB(&p.af.r8.high, p.hl.r8.low, test_flag(&p, CARRY)); }
int SBC_mHL() { return SUB(&p.af.r8.high, read_from_mem(p.hl.r16), test_flag(&p, CARRY)); }
int SBC_A() { return SUB(&p.af.r8.high, p.af.r8.high, test_flag(&p, CARRY)); }
int SBC_A_d8()
{
  SUB(&p.af.r8.high, read_from_mem(p.pc+1), test_flag(&p, CARRY));

  p.pc++; // Already incremented once in call to SUB
  return 8;
}

int AND(uint8_t r)
{
  p.af.r8.high &= r;
  set_flag(&p, ZERO, !p.af.r8.high);
  set_flag(&p, HALF_CARRY, 1);
  clear_flags(&p, SUBTRACT | CARRY);

  p.pc++;
  return 4;
}

int AND_B() { return AND(p.bc.r8.high); }
int AND_C() { return AND(p.bc.r8.low); }
int AND_D() { return AND(p.de.r8.high); }
int AND_E() { return AND(p.de.r8.low); }
int AND_H() { return AND(p.hl.r8.high); }
int AND_L() { return AND(p.hl.r8.low); }
int AND_mHL() { return AND(read_from_mem(p.hl.r16)); }
int AND_A() { return AND(p.af.r8.high); }
int AND_A_d8()
{
  AND(read_from_mem(p.pc+1));

  p.pc++; // Already incremented once in call to AND
  return 8;
}

int OR(uint8_t r)
{
  p.af.r8.high |= r;
  set_flag(&p, ZERO, !p.af.r8.high);
  clear_flags(&p, SUBTRACT | HALF_CARRY | CARRY);

  p.pc++;
  return 4;
}

int OR_B() { return OR(p.bc.r8.high); }
int OR_C() { return OR(p.bc.r8.low); }
int OR_D() { return OR(p.de.r8.high); }
int OR_E() { return OR(p.de.r8.low); }
int OR_H() { return OR(p.hl.r8.high); }
int OR_L() { return OR(p.hl.r8.low); }
int OR_mHL() { return OR(read_from_mem(p.hl.r16)); }
int OR_A() { return OR(p.af.r8.high); }
int OR_A_d8()
{
  OR(read_from_mem(p.pc+1));

  p.pc++; // Already incremented once in call to OR
  return 8;
}

int LDH_a8_A()
{
  uint16_t addr = 0xFF00 + read_from_mem(p.pc+1);
  write_to_mem(addr, p.af.r8.high);

  p.pc += 2;
  return 12;
}

int LDH_A_a8()
{
  uint16_t addr = 0xFF00 + read_from_mem(p.pc+1);
  p.af.r8.high = read_from_mem(addr);

  p.pc += 2;
  return 12;
}

int LD_SP_HL()
{
  p.sp = p.hl.r16;

  p.pc += 1;
  return 8;
}

int CP(uint8_t r)
{
  uint8_t a = p.af.r8.high;

  set_flag(&p, SUBTRACT, 1);
  set_flag(&p, ZERO, a == r);
  set_flag(&p, HALF_CARRY, (a & 0xF) < (r & 0xF));
  set_flag(&p, CARRY, a < r);
}

int CP_d8()
{
  CP(read_from_mem(p.pc+1));
  p.pc += 2;
  return 8;
}

int CP_reg(uint8_t r)
{
  CP(r);
  p.pc += 1;
  return 4;
}

int CP_B() { return CP_reg(p.bc.r8.high); }
int CP_C() { return CP_reg(p.bc.r8.low); }
int CP_D() { return CP_reg(p.de.r8.high); }
int CP_E() { return CP_reg(p.de.r8.low); }
int CP_H() { return CP_reg(p.hl.r8.high); }
int CP_L() { return CP_reg(p.hl.r8.low); }
int CP_HL() { return CP_reg(read_from_mem(p.hl.r16)); }
int CP_A() { return CP_reg(p.af.r8.high); }

int DAA()
{
  uint8_t* acc = &p.af.r8.high;

  // After an addition, adjust if (half-)carry occurred or if result is out of bounds
  if (!test_flag(&p, SUBTRACT))
  {
    if (test_flag(&p, CARRY) || *acc > 0x99)
    {
      *acc += 0x60;
      set_flag(&p, CARRY, 1);
    }
    if (test_flag(&p, HALF_CARRY) || (*acc & 0x0f) > 0x09)
      *acc += 0x6;
  }
  // After a subtraction, only adjust if (half-)carry occurred
  else
  {
      if (test_flag(&p, CARRY))
        *acc -= 0x60;
      if (test_flag(&p, HALF_CARRY))
        *acc -= 0x6;
  }

  set_flag(&p, ZERO, *acc == 0);
  clear_flags(&p, HALF_CARRY);

  p.pc++;
  return 4;
}

int CPL()
{
  ~p.af.r8.high;

  set_flag(&p, SUBTRACT, 1);
  set_flag(&p, HALF_CARRY, 1);

  p.pc++;
  return 4;
}

int CCF()
{
  // Complement carry flag
  set_flag(&p, CARRY, !test_flag(&p, CARRY));
  clear_flags(&p, SUBTRACT | HALF_CARRY);

  p.pc++;
  return 4;
}

int SCF()
{
  set_flag(&p, CARRY, 1);
  clear_flags(&p, SUBTRACT | HALF_CARRY);

  p.pc++;
  return 4;
}

op prefix_operations[NUM_OPS] = {
  RLC_B,            // 0x00
  RLC_C,            // 0x01
  RLC_D,            // 0x02
  RLC_E,            // 0x03
  RLC_H,            // 0x04
  RLC_L,            // 0x05
  RLC_HL,           // 0x06
  RLC_A,            // 0x07
  RRC_B,            // 0x08
  RRC_C,            // 0x09
  RRC_D,            // 0x0a
  RRC_E,            // 0x0b
  RRC_H,            // 0x0c
  RRC_L,            // 0x0d
  RRC_HL,           // 0x0e
  RRC_A,            // 0x0f
  RL_B,             // 0x10
  RL_C,             // 0x11
  RL_D,             // 0x12
  RL_E,             // 0x13
  RL_H,             // 0x14
  RL_L,             // 0x15
  RL_HL,            // 0x16
  RL_A,             // 0x17
  RR_B,             // 0x18
  RR_C,             // 0x19
  RR_D,             // 0x1a
  RR_E,             // 0x1b
  RR_H,             // 0x1c
  RR_L,             // 0x1d
  RR_HL,            // 0x1e
  RR_A,             // 0x1f
  SLA_B,            // 0x20
  SLA_C,            // 0x21
  SLA_D,            // 0x22
  SLA_E,            // 0x23
  SLA_H,            // 0x24
  SLA_L,            // 0x25
  SLA_HL,           // 0x26
  SLA_A,            // 0x27
  SRA_B,            // 0x28
  SRA_C,            // 0x29
  SRA_D,            // 0x2a
  SRA_E,            // 0x2b
  SRA_H,            // 0x2c
  SRA_L,            // 0x2d
  SRA_HL,           // 0x2e
  SRA_A,            // 0x2f
  SWAP_B,           // 0x30
  SWAP_C,           // 0x31
  SWAP_D,           // 0x32
  SWAP_E,           // 0x33
  SWAP_H,           // 0x34
  SWAP_L,           // 0x35
  SWAP_HL,          // 0x36
  SWAP_A,           // 0x37
  SRL_B,            // 0x38
  SRL_C,            // 0x39
  SRL_D,            // 0x3a
  SRL_E,            // 0x3b
  SRL_H,            // 0x3c
  SRL_L,            // 0x3d
  SRL_HL,           // 0x3e
  SRL_A,            // 0x3f
  test_bit_0_B,     // 0x40
  test_bit_0_C,     // 0x41
  test_bit_0_D,     // 0x42
  test_bit_0_E,     // 0x43
  test_bit_0_H,     // 0x44
  test_bit_0_L,     // 0x45
  test_bit_0_HL,    // 0x46
  test_bit_0_A,     // 0x47
  test_bit_1_B,     // 0x48
  test_bit_1_C,     // 0x49
  test_bit_1_D,     // 0x4a
  test_bit_1_E,     // 0x4b
  test_bit_1_H,     // 0x4c
  test_bit_1_L,     // 0x4d
  test_bit_1_HL,    // 0x4e
  test_bit_1_A,     // 0x4f
  test_bit_2_B,     // 0x50
  test_bit_2_C,     // 0x51
  test_bit_2_D,     // 0x52
  test_bit_2_E,     // 0x53
  test_bit_2_H,     // 0x54
  test_bit_2_L,     // 0x55
  test_bit_2_HL,    // 0x56
  test_bit_2_A,     // 0x57
  test_bit_3_B,     // 0x58
  test_bit_3_C,     // 0x59
  test_bit_3_D,     // 0x5a
  test_bit_3_E,     // 0x5b
  test_bit_3_H,     // 0x5c
  test_bit_3_L,     // 0x5d
  test_bit_3_HL,    // 0x5e
  test_bit_3_A,     // 0x5f
  test_bit_4_B,     // 0x60
  test_bit_4_C,     // 0x61
  test_bit_4_D,     // 0x62
  test_bit_4_E,     // 0x63
  test_bit_4_H,     // 0x64
  test_bit_4_L,     // 0x65
  test_bit_4_HL,    // 0x66
  test_bit_4_A,     // 0x67
  test_bit_5_B,     // 0x68
  test_bit_5_C,     // 0x69
  test_bit_5_D,     // 0x6a
  test_bit_5_E,     // 0x6b
  test_bit_5_H,     // 0x6c
  test_bit_5_L,     // 0x6d
  test_bit_5_HL,    // 0x6e
  test_bit_5_A,     // 0x6f
  test_bit_6_B,     // 0x70
  test_bit_6_C,     // 0x71
  test_bit_6_D,     // 0x72
  test_bit_6_E,     // 0x73
  test_bit_6_H,     // 0x74
  test_bit_6_L,     // 0x75
  test_bit_6_HL,    // 0x76
  test_bit_6_A,     // 0x77
  test_bit_7_B,     // 0x78
  test_bit_7_C,     // 0x79
  test_bit_7_D,     // 0x7a
  test_bit_7_E,     // 0x7b
  test_bit_7_H,     // 0x7c
  test_bit_7_L,     // 0x7d
  test_bit_7_HL,    // 0x7e
  test_bit_7_A,     // 0x7f
  reset_bit_0_B,    // 0x80
  reset_bit_0_C,    // 0x81
  reset_bit_0_D,    // 0x82
  reset_bit_0_E,    // 0x83
  reset_bit_0_H,    // 0x84
  reset_bit_0_L,    // 0x85
  reset_bit_0_HL,   // 0x86
  reset_bit_0_A,    // 0x87
  reset_bit_1_B,    // 0x88
  reset_bit_1_C,    // 0x89
  reset_bit_1_D,    // 0x8a
  reset_bit_1_E,    // 0x8b
  reset_bit_1_H,    // 0x8c
  reset_bit_1_L,    // 0x8d
  reset_bit_1_HL,   // 0x8e
  reset_bit_1_A,    // 0x8f
  reset_bit_2_B,    // 0x90
  reset_bit_2_C,    // 0x91
  reset_bit_2_D,    // 0x92
  reset_bit_2_E,    // 0x93
  reset_bit_2_H,    // 0x94
  reset_bit_2_L,    // 0x95
  reset_bit_2_HL,   // 0x96
  reset_bit_2_A,    // 0x97
  reset_bit_3_B,    // 0x98
  reset_bit_3_C,    // 0x99
  reset_bit_3_D,    // 0x9a
  reset_bit_3_E,    // 0x9b
  reset_bit_3_H,    // 0x9c
  reset_bit_3_L,    // 0x9d
  reset_bit_3_HL,   // 0x9e
  reset_bit_3_A,    // 0x9f
  reset_bit_4_B,    // 0xa0
  reset_bit_4_C,    // 0xa1
  reset_bit_4_D,    // 0xa2
  reset_bit_4_E,    // 0xa3
  reset_bit_4_H,    // 0xa4
  reset_bit_4_L,    // 0xa5
  reset_bit_4_HL,   // 0xa6
  reset_bit_4_A,    // 0xa7
  reset_bit_5_B,    // 0xa8
  reset_bit_5_C,    // 0xa9
  reset_bit_5_D,    // 0xaa
  reset_bit_5_E,    // 0xab
  reset_bit_5_H,    // 0xac
  reset_bit_5_L,    // 0xad
  reset_bit_5_HL,   // 0xae
  reset_bit_5_A,    // 0xaf
  reset_bit_6_B,    // 0xb0
  reset_bit_6_C,    // 0xb1
  reset_bit_6_D,    // 0xb2
  reset_bit_6_E,    // 0xb3
  reset_bit_6_H,    // 0xb4
  reset_bit_6_L,    // 0xb5
  reset_bit_6_HL,   // 0xb6
  reset_bit_6_A,    // 0xb7
  reset_bit_7_B,    // 0xb8
  reset_bit_7_C,    // 0xb9
  reset_bit_7_D,    // 0xba
  reset_bit_7_E,    // 0xbb
  reset_bit_7_H,    // 0xbc
  reset_bit_7_L,    // 0xbd
  reset_bit_7_HL,   // 0xbe
  reset_bit_7_A,    // 0xbf
  set_bit_0_B,      // 0xc0
  set_bit_0_C,      // 0xc1
  set_bit_0_D,      // 0xc2
  set_bit_0_E,      // 0xc3
  set_bit_0_H,      // 0xc4
  set_bit_0_L,      // 0xc5
  set_bit_0_HL,     // 0xc6
  set_bit_0_A,      // 0xc7
  set_bit_1_B,      // 0xc8
  set_bit_1_C,      // 0xc9
  set_bit_1_D,      // 0xca
  set_bit_1_E,      // 0xcb
  set_bit_1_H,      // 0xcc
  set_bit_1_L,      // 0xcd
  set_bit_1_HL,     // 0xce
  set_bit_1_A,      // 0xcf
  set_bit_2_B,      // 0xd0
  set_bit_2_C,      // 0xd1
  set_bit_2_D,      // 0xd2
  set_bit_2_E,      // 0xd3
  set_bit_2_H,      // 0xd4
  set_bit_2_L,      // 0xd5
  set_bit_2_HL,     // 0xd6
  set_bit_2_A,      // 0xd7
  set_bit_3_B,      // 0xd8
  set_bit_3_C,      // 0xd9
  set_bit_3_D,      // 0xda
  set_bit_3_E,      // 0xdb
  set_bit_3_H,      // 0xdc
  set_bit_3_L,      // 0xdd
  set_bit_3_HL,     // 0xde
  set_bit_3_A,      // 0xdf
  set_bit_4_B,      // 0xe0
  set_bit_4_C,      // 0xe1
  set_bit_4_D,      // 0xe2
  set_bit_4_E,      // 0xe3
  set_bit_4_H,      // 0xe4
  set_bit_4_L,      // 0xe5
  set_bit_4_HL,     // 0xe6
  set_bit_4_A,      // 0xe7
  set_bit_5_B,      // 0xe8
  set_bit_5_C,      // 0xe9
  set_bit_5_D,      // 0xea
  set_bit_5_E,      // 0xeb
  set_bit_5_H,      // 0xec
  set_bit_5_L,      // 0xed
  set_bit_5_HL,     // 0xee
  set_bit_5_A,      // 0xef
  set_bit_6_B,      // 0xf0
  set_bit_6_C,      // 0xf1
  set_bit_6_D,      // 0xf2
  set_bit_6_E,      // 0xf3
  set_bit_6_H,      // 0xf4
  set_bit_6_L,      // 0xf5
  set_bit_6_HL,     // 0xf6
  set_bit_6_A,      // 0xf7
  set_bit_7_B,      // 0xf8
  set_bit_7_C,      // 0xf9
  set_bit_7_D,      // 0xfa
  set_bit_7_E,      // 0xfb
  set_bit_7_H,      // 0xfc
  set_bit_7_L,      // 0xfd
  set_bit_7_HL,     // 0xfe
  set_bit_7_A,      // 0xff
};

op operations[NUM_OPS] = {
  NOP,              // 0x00
  LD_BC,            // 0x01
  LD_mBC_A,         // 0x02
  INC_BC,           // 0x03
  INC_B,            // 0x04
  DEC_B,            // 0x05
  LD_B,             // 0x06
  RLCA,             // 0x07
  LD_a16_SP,        // 0x08
  ADD_HL_BC,        // 0x09
  LD_A_mBC,         // 0x0a
  DEC_BC,           // 0x0b
  INC_C,            // 0x0c
  DEC_C,            // 0x0d
  LD_C,             // 0x0e
  RRCA,             // 0x0f
  STOP,             // 0x10
  LD_DE,            // 0x11
  LD_mDE_A,         // 0x12
  INC_DE,           // 0x13
  INC_D,            // 0x14
  DEC_D,            // 0x15
  LD_D,             // 0x16
  RLA,              // 0x17
  JR,               // 0x18
  ADD_HL_DE,        // 0x19
  LD_A_mDE,         // 0x1a
  DEC_DE,           // 0x1b
  INC_E,            // 0x1c
  DEC_E,            // 0x1d
  LD_E,             // 0x1e
  RRA,              // 0x1f
  JR_NZ,            // 0x20
  LD_HL,            // 0x21
  LD_mHL_A_inc,     // 0x22
  INC_HL,           // 0x23
  INC_H,            // 0x24
  DEC_H,            // 0x25
  LD_H,             // 0x26
  DAA,              // 0x27
  JR_Z,             // 0x28
  ADD_HL_HL,        // 0x29
  LD_A_mHL_inc,     // 0x2a
  DEC_HL,           // 0x2b
  INC_L,            // 0x2c
  DEC_L,            // 0x2d
  LD_L,             // 0x2e
  CPL,              // 0x2f
  JR_NC,            // 0x30
  LD_SP,            // 0x31
  LD_mHL_A_dec,     // 0x32
  INC_SP,           // 0x33
  INC_mHL,          // 0x34
  DEC_mHL,          // 0x35
  LD_mHL,           // 0x36
  SCF,              // 0x37
  JR_C,             // 0x38
  ADD_HL_SP,        // 0x39
  LD_A_mHL_dec,     // 0x3a
  DEC_SP,           // 0x3b
  INC_A,            // 0x3c
  DEC_A,            // 0x3d
  LD_A,             // 0x3e
  CCF,              // 0x3f
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
  XOR_HL,           // 0xae
  XOR_A,            // 0xaf
  OR_B,             // 0xb0
  OR_C,             // 0xb1
  OR_D,             // 0xb2
  OR_E,             // 0xb3
  OR_H,             // 0xb4
  OR_L,             // 0xb5
  OR_mHL,           // 0xb6
  OR_A,             // 0xb7
  CP_B,             // 0xb8
  CP_C,             // 0xb9
  CP_D,             // 0xba
  CP_E,             // 0xbb
  CP_H,             // 0xbc
  CP_L,             // 0xbd
  CP_HL,            // 0xbe
  CP_A,             // 0xbf
  RET_NZ,           // 0xc0
  POP_BC,           // 0xc1
  JP_NZ,            // 0xc2
  JP,               // 0xc3
  CALL_NZ,          // 0xc4
  PUSH_BC,          // 0xc5
  ADD_A_d8,         // 0xc6
  RST_00,           // 0xc7
  RET_Z,            // 0xc8
  RET,              // 0xc9
  JP_Z,             // 0xca
  not_implemented,  // 0xcb (Indicates prefix op)
  CALL_Z,           // 0xcc
  CALL,             // 0xcd
  ADC_A_d8,         // 0xce
  RST_08,           // 0xcf
  RET_NC,           // 0xd0
  POP_DE,           // 0xd1
  JP_NC,            // 0xd2
  not_implemented,  // 0xd3 (does not exist)
  CALL_NC,          // 0xd4
  PUSH_DE,          // 0xd5
  SUB_A_d8,         // 0xd6
  RST_10,           // 0xd7
  RET_C,            // 0xd8
  RETI,             // 0xd9
  JP_C,             // 0xda
  not_implemented,  // 0xdb (does not exist)
  CALL_C,           // 0xdc
  not_implemented,  // 0xdd (does not exist)
  SBC_A_d8,         // 0xde
  RST_18,           // 0xdf
  LDH_a8_A,         // 0xe0
  POP_HL,           // 0xe1
  LD_mC_A,          // 0xe2
  not_implemented,  // 0xe3 (does not exist)
  not_implemented,  // 0xe4 (does not exist)
  PUSH_HL,          // 0xe5
  AND_A_d8,         // 0xe6
  RST_20,           // 0xe7
  ADD_SP_r8,        // 0xe8
  JP_HL,            // 0xe9
  LD_a16_A,         // 0xea
  not_implemented,  // 0xeb (does not exist)
  not_implemented,  // 0xec (does not exist)
  not_implemented,  // 0xed (does not exist)
  XOR_A_d8,         // 0xee
  RST_28,           // 0xef
  LDH_A_a8,         // 0xf0
  POP_AF,           // 0xf1
  LD_A_mC,          // 0xf2
  DI,               // 0xf3
  not_implemented,  // 0xf4 (does not exist)
  PUSH_AF,          // 0xf5
  OR_A_d8,          // 0xf6
  RST_30,           // 0xf7
  LDHL_SP_r8,       // 0xf8
  LD_SP_HL,         // 0xf9
  LD_A_a16,         // 0xfa
  EI,               // 0xfb
  not_implemented,  // 0xfc (does not exist)
  not_implemented,  // 0xfd (does not exist)
  CP_d8,            // 0xfe
  RST_38,           // 0xff
};
