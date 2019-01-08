#include <stdio.h>
#include <string.h>
#include "input.h"
#include "memory.h"

void init_memory()
{
  memory[0xFF05] = 0x00;
  memory[0xFF06] = 0x00;
  memory[0xFF07] = 0x00;
  memory[0xFF10] = 0x80;
  memory[0xFF11] = 0xBF;
  memory[0xFF12] = 0xF3;
  memory[0xFF14] = 0xBF;
  memory[0xFF16] = 0x3F;
  memory[0xFF17] = 0x00;
  memory[0xFF19] = 0xBF;
  memory[0xFF1A] = 0x7F;
  memory[0xFF1B] = 0xFF;
  memory[0xFF1C] = 0x9F;
  memory[0xFF1E] = 0xBF;
  memory[0xFF20] = 0xFF;
  memory[0xFF21] = 0x00;
  memory[0xFF22] = 0x00;
  memory[0xFF23] = 0xBF;
  memory[0xFF24] = 0x77;
  memory[0xFF25] = 0xF3;
  memory[0xFF26] = 0xF1;
  memory[0xFF40] = 0x91;
  memory[0xFF42] = 0x00;
  memory[0xFF43] = 0x00;
  memory[0xFF45] = 0x00;
  memory[0xFF47] = 0xFC;
  memory[0xFF48] = 0xFF;
  memory[0xFF49] = 0xFF;
  memory[0xFF4A] = 0x00;
  memory[0xFF4B] = 0x00;
  memory[0xFFFF] = 0x00;
}

void load_rom(char* filename, int size)
{
  FILE* fp;
  fp = fopen(filename, "rb");
  fread(memory, size, 1, fp);
  fclose(fp);
}

uint8_t read_from_mem(uint16_t addr)
{
  return memory[addr];
}

void write_to_mem(uint16_t addr, uint8_t data)
{
  // Serial
  if (addr == 0xFF02 && data == 0x81)
    printf("%c", read_from_mem(0xFF01));

  // DMA
  if (addr == 0xFF46)
    memcpy(&memory[SPRITE_ATTRIB_MEM], &memory[data << 8], 160);  // Copy 40 sprites, each with a size of 4 bytes

  if (addr == 0xFF00)
  {
    if (data == 0x10)
      data |= create_joypad_reg_value(joypad_column_1);
    else if (data == 0x20)
      data |= create_joypad_reg_value(joypad_column_2);
  }

  memory[addr] = data;
}

uint8_t* get_mem_ref(uint16_t addr)
{
  return &memory[addr];
}

