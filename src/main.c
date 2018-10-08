#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "processor.h"
#include "utils.h"
#include "memory.h"

#define ROM_FILE "roms/tetris.gb"
#define TEST_ROM "roms/test.gb"
#define BOOTSTRAP_FILE "roms/DMG_ROM.bin"
#define ROM_SIZE 32768

int main(int argc, char* argv[])
{
  init_proc();
  init_memory();

  load_rom(TEST_ROM, ROM_SIZE);

  int break_point = 0;

  // Main loop
  while (1)
  {
    /*
    if ((p.pc == 0x...) || break_point)
    {
      break_point = 1;
      print_debug_info();
      getchar();
    }
    */

    int res = run_operation();

    if (res < 0)
    {
      printf("Stopped at pc: 0x%x\n", p.pc);
      printf("Unrecognized op: 0x%x\n", (uint8_t) memory[p.pc]);
      return -1;
    }

  }

  return 0;
}
