#include "memory.h"

uint8_t read_from_mem(uint16_t addr)
{
  return memory[addr];
}

void write_to_mem(uint16_t addr, uint8_t data)
{
  memory[addr] = data;
}
