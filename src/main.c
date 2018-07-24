#include <stdio.h>
#include "constants.h"
#include "processor.h"
#include "utils.h"

#define ROM_FILE "roms/tetris.gb"
#define ROM_SIZE 32768

int main(int argc, char* argv[])
{
  char memory[MEMORY_SIZE];

  FILE* fp;
  fp = fopen(ROM_FILE, "rb");
  fread(memory, ROM_SIZE, 1, fp);
  fclose(fp);

  proc p;
  p.pc = 0;
  p.mem = memory;

  // main loop
  while (1)
  {
    int res = run_operation(&p, memory[p.pc]); 
    if (res < 0)
    {
      printf("Unrecognized op: %x\n", (uint8_t) memory[p.pc]);
      return -1;
    }
  }

  return 0;
}
