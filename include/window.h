#ifndef _WINDOW_H
#define _WINDOW_H

#include <stdint.h>
#include <SDL2/SDL.h>

void create_window();
void set_draw_color(uint8_t* color_rgb);
void draw_point(int, int);
void drawing_done();

#endif

