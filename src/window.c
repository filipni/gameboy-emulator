#include "window.h"

SDL_Window *window;
SDL_Renderer *renderer;

void create_window()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(256, 256, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}

void set_draw_color(uint8_t* color_rgb)
{
  SDL_SetRenderDrawColor(renderer, color_rgb[0], color_rgb[1], color_rgb[2], 255);  
}

void draw_point(int x, int y)
{
  SDL_RenderDrawPoint(renderer, x, y);
}

void drawing_done()
{
  SDL_RenderPresent(renderer);
}

