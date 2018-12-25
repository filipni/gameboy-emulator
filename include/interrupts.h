#ifndef _IRQ_H
#define _IRQ_H

#include <stdio.h>
#include "processor.h"
#include "memory.h"

#define IF_REG 0xFF0F
#define IE_REG 0xFFFF

#define V_BLANK_MASK  0x01
#define LCDC_MASK     0x02 
#define TIMER_MASK    0x04 
#define SERIAL_MASK   0x08 
#define JOYPAD_MASK   0x10 

#define IRQ_VEC_BASE_ADDR 0x0040

int ei_scheduled;
int di_scheduled;

int irq_handling();
int call_irq_routine(uint8_t);

#endif
