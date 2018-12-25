#include "processor.h"
#include "utils.h"
#include "memory.h"
#include "instructions.h"
#include "interrupts.h"

#include <string.h>
#include <stdio.h>

void init_proc()
{
  p.af.r16 = 0x01B0;
  p.bc.r16 = 0x0013;
  p.de.r16 = 0x00D8;
  p.hl.r16 = 0x014D;
  p.sp = 0xFFFE;
  p.pc = 0x0100;

  p.interrupts_enabled = 0;
}

void print_debug_info()
{
  printf("FLAGS\n");
  printf("-----\n");
  printf("Zero:    %x\n", test_flag(&p, ZERO));
  printf("Sub:     %x\n", test_flag(&p, SUBTRACT));
  printf("H-carry: %x\n", test_flag(&p, HALF_CARRY));
  printf("Carry:   %x\n\n", test_flag(&p, CARRY));

  printf("MEMORY LOCATIONS\n");
  printf("----------------\n");
  printf("IE: %02x\n", memory[IE_REG]);
  printf("IF: %02x\n\n", memory[IF_REG]);

  printf("REGISTERS\n");
  printf("---------\n");
  printf("AF: %04x\n", p.af.r16);
  printf("BC: %04x\n", p.bc.r16);
  printf("DE: %04x\n", p.de.r16);
  printf("HL: %04x\n", p.hl.r16);
  printf("SP: %04x\n", p.sp);
  printf("PC: %04x\n\n", p.pc);

  printf("===========\n");
}

int run_operation()
{
  int res = handle_interrupts();
  if (res > 0)
    return res;

  uint8_t op_code = read_from_mem(p.pc);
  if (op_code == 0xcb)  // Check if op is from the prefix instruction set
  {
    op_code = read_from_mem(p.pc+1);
    return prefix_operations[op_code](p);
  }
  else
    return operations[op_code](p);
}

