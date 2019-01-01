#include "ppu.h"
#include "memory.h"
#include "constants.h"
#include "display.h"
#include <stdio.h>

#define SCX_REG 0xff43
#define SCY_REG 0xff42
#define LY_REG 0xff44

#define NUM_ROWS 32
#define NUM_COLS 32

const uint8_t WHITE[] = {155, 188, 15};
const uint8_t LIGHT_GREY[] = {139, 172, 15};
const uint8_t DARK_GREY[] = {48, 98, 48};
const uint8_t BLACK[] = {15, 56, 15};

const uint8_t* RGB_VALUES[] = {WHITE, LIGHT_GREY, DARK_GREY, BLACK};

static void draw_tile(uint8_t index, int x, int y)
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

      set_draw_color(RGB_VALUES[color_index]);
      draw_point(x + 7 - j, y + i/2);  // 7 - j, so that we draw the high bits before the lower ones.
    }
  }
}

int draw_window(int map_index)
{
  drawing_init();
  if (map_index < 0 || map_index > 1)
    return -1;

  uint16_t map_address = map_index ? TILE_MAP_1 : TILE_MAP_0;

  for (int row = 0; row < NUM_ROWS; row++)
  {
    for (int column = 0; column < NUM_COLS; column++)
    {
      int tile_index = memory[map_address + row * NUM_COLS + column];
      draw_tile(tile_index, column*8, row*8);  // Each tile is 8x8 bits.
    }
  }

  set_offset(memory[SCX_REG], memory[SCY_REG]);

  drawing_done();
  return 0;
}

