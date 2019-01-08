#include <SDL2/SDL.h>

#define ROWS_PER_JOYPAD_COLUMN 4

int joypad_column_1[ROWS_PER_JOYPAD_COLUMN];
int joypad_column_2[ROWS_PER_JOYPAD_COLUMN];

enum keys_joypad_column_1
{
  A,
  B,
  SELECT,
  START
};

enum keys_joypad_column_2
{
  RIGHT,
  LEFT,
  UP,
  DOWN
};

void handle_keyboard_input(SDL_Event*);
uint8_t create_joypad_reg_value(int joypad_column[]);

