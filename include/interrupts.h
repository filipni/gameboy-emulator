#ifndef _IRQ_H
#define _IRQ_H

#include <stdint.h>

#define IF_REG 0xFF0F  // Interrupts Fired
#define IE_REG 0xFFFF  // Interrupts Enabled

#define IRQ_VEC_BASE_ADDR 0x0040

int ei_scheduled;
int di_scheduled;

int irq_handling();
int call_irq_routine(uint8_t);

#endif
