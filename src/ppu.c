#include "ppu.h"
#include <stdio.h>

void draw_tile(uint8_t index)
{
  uint16_t tile_address = VIDEO_RAM + index * TILE_SIZE_BYTES;
  for (int i = 0; i < TILE_SIZE_BYTES; i += 2)
  {
    uint8_t low_color_bits = memory[tile_address + i];
    uint8_t high_color_bits = memory[tile_address + i + 1];

    for (int j = 0; j < 8; j++)
    {
      uint8_t low_bit = (0x80 & low_color_bits) >> 7;
      uint8_t high_bit = (0x80 & high_color_bits) >> 6;

      uint8_t color_index = high_bit | low_bit;
      color_index ? printf("*") : printf("_");

      low_color_bits <<= 1;
      high_color_bits <<= 1;
    }
    printf("\n");
  }
  printf("\n");
}
