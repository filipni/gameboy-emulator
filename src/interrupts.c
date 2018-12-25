#include "interrupts.h"

int ei_scheduled = 0;
int di_scheduled = 0;

int call_irq_routine(uint8_t irq_num)
{
    p.interrupts_enabled = 0;

    // Push pc to stack.
    write_to_mem(p.sp-1, (p.pc & 0xFF00) >> 8);  // High byte
    write_to_mem(p.sp-2, p.pc & 0xFF);           // Low byte
    p.sp -= 2;

    // Clear interrupt and jumb to subroutine.
    memory[IF_REG] &= ~(1 << irq_num);
    p.pc = IRQ_VEC_BASE_ADDR + 8 * irq_num;

    return 12;
}

int irq_handling()
{
  int irq_handling_overhead = 0;

  uint8_t irqs_to_handle = memory[IF_REG] & memory[IE_REG];
  if (p.interrupts_enabled && irqs_to_handle)
  {
    uint8_t irq_num = 0;
    while ((1 << irq_num & irqs_to_handle) == 0)
      irq_num++;
    irq_handling_overhead += call_irq_routine(irq_num);
  }

  // Interupts are not enabled/disabled until the instruction after EI/DI is executed.
  if (ei_scheduled)
    p.interrupts_enabled = 1;
  if (di_scheduled)
    p.interrupts_enabled = 0;
  ei_scheduled = di_scheduled = 0;

  return irq_handling_overhead;
}

