#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "processor.h"
#include "utils.h"

#define ROM_FILE "roms/tetris.gb"
#define ROM_SIZE 32768

void print_debug_info(proc* p)
{
  printf("FLAGS\n");
  printf("-----\n");
  printf("Zero:    %x\n", p->f.z);
  printf("Sub:     %x\n", p->f.n);
  printf("Carry:   %x\n", p->f.c);
  printf("H-carry: %x\n\n", p->f.h);

  printf("REGISTERS\n");
  printf("---------\n");
  printf("AF: %x\n", p->af.r16);
  printf("BC: %x\n", p->bc.r16);
  printf("DE: %x\n", p->de.r16);
  printf("HL: %x\n", p->hl.r16);
  printf("PC: %x\n", p->pc);
  printf("SP: %x\n\n", p->sp);

  printf("===========\n");
}

int main(int argc, char* argv[])
{
  char memory[MEMORY_SIZE] = {0};

  FILE* fp;
  fp = fopen(ROM_FILE, "rb");
  fread(memory, ROM_SIZE, 1, fp);
  fclose(fp);

  proc p = {0};
  p.mem = memory;

  // main loop
  while (1)
  {
    int res = run_operation(&p, memory[p.pc]); 
    if (res < 0)
    {
      printf("Stopped at pc: 0x%x\n", p.pc);
      printf("Unrecognized op: 0x%x\n", (uint8_t) memory[p.pc]);
      return -1;
    }
  }

  return 0;
}
