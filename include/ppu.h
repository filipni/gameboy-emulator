#ifndef _PPU_H
#define _PPU_H

#include <stdint.h>

void draw_tile(uint8_t, int, int);
int draw_window(int);

#define SCX_REG 0xff43
#define SCY_REG 0xff42
#define LY_REG 0xff44

#endif

