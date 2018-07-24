#include "utils.h"

uint16_t generate_address(uint8_t low_byte, uint8_t high_byte)
{
  uint16_t res = (uint16_t) high_byte << 8 | (uint16_t) low_byte; 
  return res;
}
