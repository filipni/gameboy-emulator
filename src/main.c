#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "processor.h"
#include "utils.h"
#include "memory.h"
#include "display.h"
#include "interrupts.h"
#include <SDL2/SDL.h>

#define ROM_FILE "roms/tetris.gb"
#define TEST_ROM "roms/test.gb"
#define BOOTSTRAP_FILE "roms/DMG_ROM.bin"
#define ROM_SIZE 0x8000
#define BOOTSTRAP_SIZE 0x100

#define CPU_SPEED_HZ 4190000
#define V_BLANK_HZ 59.73
#define V_BLANK_CYCLES CPU_SPEED_HZ / V_BLANK_HZ

int cycle_counter = 0;

void sig_handler(int signo)
{
  print_debug_info();
  exit(0);
}

int main(int argc, char* argv[])
{
  signal(SIGINT, sig_handler);
  create_display();

  init_proc();
  init_memory();

  load_rom(ROM_FILE, ROM_SIZE);
  load_rom(BOOTSTRAP_FILE, BOOTSTRAP_SIZE);

  int breakpoint = 0x100;
  int step_enabled = 0;

  SDL_Event e;
  int quit = 0;

  // Main loop
  while (!quit)
  {
    /*
    if ((p.pc == breakpoint) || step_enabled)
    {
      print_debug_info();
      char c = getchar();
      if (c == 'n')
      {
        printf("Enter new breakpoint: ");
        scanf("%x", &breakpoint);
        step_enabled = 0;
      }
      else
        step_enabled = 1;
    }
    */

    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
        quit = 1;
    }

    if (p.pc == 0x100)
      load_rom(ROM_FILE, ROM_SIZE);

    int res = run_operation();
    cycle_counter += res;


    memory[0xFF44]++;

    if (cycle_counter >= V_BLANK_CYCLES)
    {
      cycle_counter = 0;
      memory[IF_REG] |= 1;

      draw_to_display(0);
      SDL_Delay(15);
    }

    if (res < 0)
    {
      printf("Stopped at pc: 0x%x\n", p.pc);
      printf("Unrecognized op: 0x%x\n", (uint8_t) memory[p.pc]);
      return -1;
    }

  }

  return 0;
}
