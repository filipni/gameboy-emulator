#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include "processor.h"

#define ZERO         0x80
#define SUBTRACT     0x40
#define HALF_CARRY   0x20
#define CARRY        0x10

uint16_t generate_address(uint8_t, uint8_t);
void _set_flag(proc*, uint8_t);
void clear_flags(proc*, uint8_t);
void set_flag(proc*, uint8_t, uint8_t);
uint8_t test_flag(proc*, uint8_t);

#endif
