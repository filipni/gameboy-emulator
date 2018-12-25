#include "interrupts.h"

int interrupt_allowed(uint8_t irq_mask)
{
  return memory[IF_REG] & irq_mask && memory[IE_REG] & irq_mask; 
}

void handle_interrupts()
{
  if (p.interrupts_enabled && memory[IF_REG] != 0)
  {
    p.interrupts_enabled = 0;

    // Push pc to stack
    write_to_mem(p.sp-1, (p.pc & 0xFF00) >> 8);  // High byte
    write_to_mem(p.sp-2, p.pc & 0xFF);           // Low byte
    p.sp -= 2;
    
    if (interrupt_allowed(V_BLANK_MASK))
      p.pc = V_BLANK_IRQ_VECTOR_ADDR;
    else if (interrupt_allowed(LCDC_MASK))
      p.pc = LCDC_IRQ_VECTOR_ADDR;
    else if (interrupt_allowed(TIMER_MASK))
      p.pc = TIMER_IRQ_VECTOR_ADDR;
    else if (interrupt_allowed(SERIAL_MASK))
      p.pc = SERIAL_IRQ_VECTOR_ADDR;
    else if (interrupt_allowed(JOYPAD_MASK))
      p.pc = JOYPAD_IRQ_VECTOR_ADDR;
  }
  memory[IF_REG] = 0;
}
