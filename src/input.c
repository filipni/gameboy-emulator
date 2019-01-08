#include "input.h"

int joypad_column_1[ROWS_PER_JOYPAD_COLUMN] = {0};
int joypad_column_2[ROWS_PER_JOYPAD_COLUMN] = {0};

void handle_keyboard_input(SDL_Event* e)
{
  uint8_t key_down = e->type == SDL_KEYDOWN;

  switch(e->key.keysym.sym)
  {
    case SDLK_UP:
      joypad_column_2[UP] = key_down;
      break;
    case SDLK_DOWN:
      joypad_column_2[DOWN] = key_down;
      break;
    case SDLK_LEFT:
      joypad_column_2[LEFT] = key_down;
      break;
    case SDLK_RIGHT:
      joypad_column_2[RIGHT] = key_down;
      break;
    case SDLK_a:
      joypad_column_1[B] = key_down;
      break;
    case SDLK_s:
      joypad_column_1[A] = key_down;
      break;
    case SDLK_SPACE:
      joypad_column_1[SELECT] = key_down;
      break;
    case SDLK_RETURN:
      joypad_column_1[START] = key_down;
      break;
  }
}

uint8_t create_joypad_reg_value(int joypad_column[])
{
  uint8_t result = 0;
  for (int i = 0; i < ROWS_PER_JOYPAD_COLUMN; i++)
  {
    uint8_t bitmask = 1 << i;
    if (joypad_column[i])
      result |= bitmask;
  }
  return ~result;  // Invert because of pull-up resistor on joypad register
}


