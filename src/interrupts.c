#include "interrupts.h"

int pending_interrupts_enabled = 0;
int pending_interrupts_disabled = 0;

int handle_interrupts()
{
  uint8_t interrupts_to_handle = memory[IF_REG] & memory[IE_REG];
  int irq_handling_overhead = 0;
  if (p.interrupts_enabled && interrupts_to_handle)
  {
    p.interrupts_enabled = 0;

    // Push pc to stack
    write_to_mem(p.sp-1, (p.pc & 0xFF00) >> 8);  // High byte
    write_to_mem(p.sp-2, p.pc & 0xFF);           // Low byte
    p.sp -= 2;
    
    if (interrupts_to_handle & V_BLANK_MASK)
    {
      memory[IF_REG] &= ~V_BLANK_MASK;
      p.pc = V_BLANK_IRQ_VECTOR_ADDR;
    }
    else if (interrupts_to_handle & LCDC_MASK)
    {
      memory[IF_REG] &= ~LCDC_MASK;
      p.pc = LCDC_IRQ_VECTOR_ADDR;
    }
    else if (interrupts_to_handle & TIMER_MASK)
    {
      memory[IF_REG] &= ~TIMER_MASK;
      p.pc = TIMER_IRQ_VECTOR_ADDR;
    }
    else if (interrupts_to_handle & SERIAL_MASK)
    {
      memory[IF_REG] &= ~SERIAL_MASK;
      p.pc = SERIAL_IRQ_VECTOR_ADDR;
    }
    else if (interrupts_to_handle & JOYPAD_MASK)
    {
      memory[IF_REG] &= ~JOYPAD_MASK;
      p.pc = JOYPAD_IRQ_VECTOR_ADDR;
    }

    irq_handling_overhead = 12;
  }

  // Interupts are not enabled/disabled until the instruction after EI/DI is executed.
  if (pending_interrupts_enabled)
  {
    p.interrupts_enabled = 1;
    pending_interrupts_enabled = 0;
  }
  if (pending_interrupts_disabled)
  {
    p.interrupts_enabled = 0;
    pending_interrupts_disabled = 0;
  }

  return irq_handling_overhead;
}
