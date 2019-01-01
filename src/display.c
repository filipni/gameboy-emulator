#include "display.h"
#include "memory.h"
#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Surface* window_surface;
SDL_Renderer* tile_map_renderer;
SDL_Surface* tile_map_surface;

const uint8_t WHITE[] = {155, 188, 15};
const uint8_t LIGHT_GREY[] = {139, 172, 15};
const uint8_t DARK_GREY[] = {48, 98, 48};
const uint8_t BLACK[] = {15, 56, 15};
const uint8_t* RGB_VALUES[] = {WHITE, LIGHT_GREY, DARK_GREY, BLACK};

#define WINDOW_NAME "GameBoyEmu"

#define SCX_REG 0xff43
#define SCY_REG 0xff42

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 144

#define TILE_SIZE_BYTES 16
#define TILE_MAP_WIDTH_PIXELS 256
#define TILE_MAP_HEIGHT_PIXELS 256
#define TILE_MAP_WIDTH_BYTES TILE_MAP_WIDTH_PIXELS / 8
#define TILE_MAP_HEIGHT_BYTES TILE_MAP_HEIGHT_PIXELS / 8

void create_display()
{
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow(WINDOW_NAME, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0);
  window_surface = SDL_GetWindowSurface(window);
  SDL_PixelFormat* fmt = window_surface->format;

  tile_map_surface = SDL_CreateRGBSurface(
      0,
      TILE_MAP_WIDTH_PIXELS,
      TILE_MAP_HEIGHT_PIXELS,
      fmt->BitsPerPixel,
      fmt->Rmask,
      fmt->Gmask,
      fmt->Bmask,
      fmt->Amask);
  tile_map_renderer = SDL_CreateSoftwareRenderer(tile_map_surface);
}

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
      SDL_RenderDrawPoint(tile_map_renderer, x + 7 - j, y + i/2);  // 7 - j, so that we draw the high bits before the lower ones.
    }
  }
}

int draw_to_display(int map_index)
{
  // Clear renderer before drawing
  SDL_SetRenderDrawColor(tile_map_renderer, 0, 0, 0, 255);
  SDL_RenderClear(tile_map_renderer);

  if (map_index < 0 || map_index > 1)
    return -1;

  uint16_t map_address = map_index ? TILE_MAP_1 : TILE_MAP_0;

  for (int row = 0; row < TILE_MAP_HEIGHT_BYTES; row++)
  {
    for (int column = 0; column < TILE_MAP_WIDTH_BYTES; column++)
    {
      int tile_index = memory[map_address + row * TILE_MAP_WIDTH_BYTES + column];
      draw_tile(tile_index, column*8, row*8);  // Each tile is 8x8 bits.
    }
  }

  // Crop tile map to fit display
  SDL_Rect rect = {memory[SCX_REG], memory[SCY_REG], DISPLAY_WIDTH, DISPLAY_HEIGHT};
  SDL_BlitSurface(tile_map_surface, &rect, window_surface, NULL);
  SDL_UpdateWindowSurface(window);

  return 0;
}

void set_draw_color(const uint8_t* color_rgb)
{
  SDL_SetRenderDrawColor(tile_map_renderer, color_rgb[0], color_rgb[1], color_rgb[2], 255);
}

