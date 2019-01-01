#ifndef _WINDOW_H
#define _WINDOW_H

#include <stdint.h>

void create_display();
int draw_to_display(int);
void set_draw_color(const uint8_t* color_rgb);
static void draw_tile(uint8_t, int, int);

#endif

