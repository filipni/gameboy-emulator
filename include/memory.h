#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>

#define MEMORY_SIZE         0x10000

#define ROM_BANK_O          0x0
#define SWITCHABLE_ROM_BANK 0x4000
#define VIDEO_RAM           0x8000
#define SWITCHABLE_RAM_BANK 0xA000
#define INTERNAL_RAM_8KB    0xC000
#define ECHO_INTERNAL_RAM   0xE000
#define SPRITE_ATTRIB_MEM   0xFE00
#define EMPTY_IO_1          0xFEA0
#define IO_PORTS            0xFF00
#define EMPTY_IO_2          0xFF4C
#define INTERNAL_RAM        0xFF80
#define INTER_ENABLE_REG    0xFFFF

// Different sections in video ram
#define TILE_SET_1          0x8000
#define TILE_SET_SHARED     0x8800
#define TILE_SET_0          0x9000
#define TILE_MAP_0          0x9800
#define TILE_MAP_1          0x9C00

uint8_t memory[MEMORY_SIZE];

void init_memory();
void load_rom();

uint8_t read_from_mem(uint16_t addr);
void write_to_mem(uint16_t addr, uint8_t data);
uint8_t* get_mem_ref(uint16_t addr);

#endif
