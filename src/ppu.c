#include "ppu.h"

uint8_t WHITE_RGB[] = {155, 188, 15};
uint8_t LIGHT_GREY_RGB[] = {139, 172, 15};
uint8_t DARK_GREY_RGB[] = {48, 98, 48};
uint8_t BLACK_RGB[] = {15, 56, 15};

void set_pixel_color(uint8_t color_index)
{
  switch (color_index)
  {
    case 0:
      set_draw_color(WHITE_RGB);
      break;
    case 1:
      set_draw_color(LIGHT_GREY_RGB);
      break;
    case 2:
      set_draw_color(DARK_GREY_RGB);
      break;
    case 3:
      set_draw_color(BLACK_RGB);
      break;
  }
}

void draw_tile(uint8_t index, int x, int y)
{
  uint16_t tile_address = VIDEO_RAM + index * TILE_SIZE_BYTES;
  for (int i = 0; i < TILE_SIZE_BYTES; i += 2)
  {
    uint8_t low_color_bits = memory[tile_address + i];
    uint8_t high_color_bits = memory[tile_address + i + 1];

    for (int j = 0; j < 8; j++)
    {
      uint8_t bitmask = 1 << j;
      uint8_t low_bit = low_color_bits & bitmask ? 1 : 0;
      uint8_t high_bit = high_color_bits & bitmask ? 2 : 0;

      uint8_t color_index = high_bit | low_bit;

      set_pixel_color(color_index);
      draw_point(x + 7 - j, y + i/2);  // 7 - j, so that we draw the high bits before the lower ones.
    }
  }
}

int draw_map(int map_index)
{
  if (map_index < 0 || map_index > 1)
    return -1;

  uint16_t map_address = map_index ? TILE_MAP_1 : TILE_MAP_0;

  // Each map is 32x32 bytes.
  for (int i = 0; i < 32; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      int tile_index = memory[map_address + i + j*32];  // The maps are saved column by column in memory, so each row element is offset with 32 bytes.
      draw_tile(tile_index, i*8, j*8);  // Each tile is 8x8 bits.
    }
  }

  drawing_done();
  return 0;
}

