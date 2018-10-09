#include "utils.h"
#include <stdio.h>

uint16_t generate_address(uint8_t low_byte, uint8_t high_byte)
{
  uint16_t res = (uint16_t) high_byte << 8 | (uint16_t) low_byte;
  return res;
}

void _set_flag(proc* p, uint8_t f)
{
  p->af.r8.low |= f;
}

void clear_flags(proc* p, uint8_t f)
{
  p->af.r8.low &= ~f;
}

void set_flag(proc* p, uint8_t f, uint8_t val)
{
  if (val)
    _set_flag(p, f);
  else
    clear_flags(p, f);
}

uint8_t test_flag(proc* p, uint8_t f)
{
  return (p->af.r8.low & f) != 0;
}

