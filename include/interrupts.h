#ifndef _IRQ_H
#define _IRQ_H

#include <stdio.h>
#include "processor.h"
#include "memory.h"
#include "utils.h"

#define IF_REG 0xFF0F
#define IE_REG 0xFFFF

#define V_BLANK_MASK  0x01
#define LCDC_MASK     0x02 
#define TIMER_MASK    0x04 
#define SERIAL_MASK   0x08 
#define JOYPAD_MASK   0x10 

#define V_BLANK_IRQ_VECTOR_ADDR 0x0040
#define LCDC_IRQ_VECTOR_ADDR 0x0048
#define TIMER_IRQ_VECTOR_ADDR 0x0050
#define SERIAL_IRQ_VECTOR_ADDR 0x0058
#define JOYPAD_IRQ_VECTOR_ADDR 0x0060

void handle_interrupts();

#endif
