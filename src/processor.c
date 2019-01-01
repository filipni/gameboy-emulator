#include "processor.h"
#include "utils.h"
#include "memory.h"
#include "instructions.h"
#include "interrupts.h"

#include <string.h>
#include <stdio.h>

void init_proc()
{
  /*
  p.af.r16 = 0x01B0;
  p.bc.r16 = 0x0013;
  p.de.r16 = 0x00D8;
  p.hl.r16 = 0x014D;
  p.sp = 0xFFFE;
  p.pc = 0x0100;
  */

  p.af.r16 = 0x0;
  p.bc.r16 = 0x0;
  p.de.r16 = 0x0;
  p.hl.r16 = 0x0;
  p.sp = 0x0;
  p.pc = 0x0;

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

  printf("INTERRUPTS\n");
  printf("----------------\n");
  printf("IME: %x\n", p.interrupts_enabled);
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
  int irq_handling_overhead = irq_handling();

  int op_overhead = 0;
  uint8_t op_code = read_from_mem(p.pc);
  if (op_code == 0xcb)  // Check if op is from the prefix instruction set
  {
    op_code = read_from_mem(p.pc+1);
    op_overhead = prefix_operations[op_code](p);
  }
  else
    op_overhead = operations[op_code](p);

  return irq_handling_overhead + op_overhead;
}

