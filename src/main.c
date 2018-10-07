#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "processor.h"
#include "utils.h"
#include "memory.h"

#define ROM_FILE "roms/tetris.gb"
#define BOOTSTRAP_FILE "roms/DMG_ROM.bin"
#define ROM_SIZE 32768

int main(int argc, char* argv[])
{
  init_proc();
  init_memory();

  load_rom(ROM_FILE, ROM_SIZE);

  // Main loop
  while (1)
  {

    //print_debug_info();
    //getchar();

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
