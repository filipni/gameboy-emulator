#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "constants.h"
#include "processor.h"
#include "utils.h"
#include "memory.h"
#include "ppu.h"
#include "window.h"

#define ROM_FILE "roms/tetris.gb"
#define TEST_ROM "roms/test.gb"
#define BOOTSTRAP_FILE "roms/DMG_ROM.bin"
#define ROM_SIZE 32768

int cycle_counter = 0;
int v_counter = 0;

void sig_handler(int signo)
{
  print_debug_info();
  exit(0);
}

int main(int argc, char* argv[])
{
  signal(SIGINT, sig_handler);
  create_window();

  init_proc();
  init_memory();

  load_rom(ROM_FILE, ROM_SIZE);

  int breakpoint = 0x100;
  int step_enabled = 0;

  // Main loop
  while (1)
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

    int res = run_operation();
    cycle_counter += res;

    if (cycle_counter >= 450)
    {
      cycle_counter = 0;
      memory[0xFF44]++;
    }

    v_counter += res;
    if (v_counter >= V_BLANK_CYCLES)
    {
      v_counter = 0;
      memory[0xFF0F] = 1;
      draw_map(0);
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
