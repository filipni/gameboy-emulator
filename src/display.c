#include "display.h"
#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Surface* window_surface;
SDL_Renderer* tile_map_renderer;
SDL_Surface* tile_map_surface;

static int x_offset;
static int y_offset;

void create_window()
{
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("GameBoyEmu", 0, 0, 160, 144, 0);
  window_surface = SDL_GetWindowSurface(window);
  SDL_PixelFormat* fmt = window_surface->format;

  tile_map_surface = SDL_CreateRGBSurface(
      0,
      256,
      256,
      fmt->BitsPerPixel,
      fmt->Rmask,
      fmt->Gmask,
      fmt->Bmask,
      fmt->Amask);
  tile_map_renderer = SDL_CreateSoftwareRenderer(tile_map_surface);
}

void drawing_init()
{
  SDL_SetRenderDrawColor(tile_map_renderer, 0, 0, 0, 255);
  SDL_RenderClear(tile_map_renderer);
}

void set_draw_color(uint8_t* color_rgb)
{
  SDL_SetRenderDrawColor(tile_map_renderer, color_rgb[0], color_rgb[1], color_rgb[2], 255);
}

void draw_point(int x, int y)
{
  SDL_RenderDrawPoint(tile_map_renderer, x, y);
}

void set_offset(int x, int y)
{
  x_offset = x;
  y_offset = y;
}

void drawing_done()
{
  SDL_Rect rect = {x_offset, y_offset, 160, 144};
  SDL_BlitSurface(tile_map_surface, &rect, window_surface, NULL);
  SDL_UpdateWindowSurface(window);
}

