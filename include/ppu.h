#ifndef _PPU_H
#define _PPU_H

#include <stdint.h>
#include "memory.h"
#include "constants.h"
#include "window.h"

void draw_tile(uint8_t, int, int);
void set_pixel_color(uint8_t);
int draw_map();

#endif

