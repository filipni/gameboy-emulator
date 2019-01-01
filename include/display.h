#ifndef _WINDOW_H
#define _WINDOW_H

#include <stdint.h>

void create_window();
void drawing_init();
void drawing_done();
void set_draw_color(const uint8_t* color_rgb);
void draw_point(int, int);
void set_offset(int, int);

#endif

