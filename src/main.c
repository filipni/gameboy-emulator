#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "processor.h"
#include "utils.h"

#define ROM_FILE "roms/tetris.gb"
#define BOOTSTRAP_FILE "roms/DMG_ROM.bin"
#define ROM_SIZE 32768

void print_debug_info(proc* p)
{
  printf("FLAGS\n");
  printf("-----\n");
  printf("Zero:    %x\n", test_flag(p, ZERO));
  printf("Sub:     %x\n", test_flag(p, SUBTRACT));
  printf("H-carry: %x\n", test_flag(p, HALF_CARRY));
  printf("Carry:   %x\n\n", test_flag(p, CARRY));

  printf("REGISTERS\n");
  printf("---------\n");
  printf("AF: %04x\n", p->af.r16);
  printf("BC: %04x\n", p->bc.r16);
  printf("DE: %04x\n", p->de.r16);
  printf("HL: %04x\n", p->hl.r16);
  printf("SP: %04x\n", p->sp);
  printf("PC: %04x\n\n", p->pc);

  printf("===========\n");
}

void init_proc(proc* p)
{
  p->af.r16 = 0x01B0;
  p->bc.r16 = 0x0013;
  p->de.r16 = 0x00D8;
  p->hl.r16 = 0x014D;
  p->sp = 0xFFFE;
  p->pc = 0x0100;

  p->mem[0xFF05] = 0x00;
  p->mem[0xFF06] = 0x00;
  p->mem[0xFF07] = 0x00;
  p->mem[0xFF10] = 0x80;
  p->mem[0xFF11] = 0xBF;
  p->mem[0xFF12] = 0xF3;
  p->mem[0xFF14] = 0xBF;
  p->mem[0xFF16] = 0x3F;
  p->mem[0xFF17] = 0x00;
  p->mem[0xFF19] = 0xBF;
  p->mem[0xFF1A] = 0x7F;
  p->mem[0xFF1B] = 0xFF;
  p->mem[0xFF1C] = 0x9F;
  p->mem[0xFF1E] = 0xBF;
  p->mem[0xFF20] = 0xFF;
  p->mem[0xFF21] = 0x00;
  p->mem[0xFF22] = 0x00;
  p->mem[0xFF23] = 0xBF;
  p->mem[0xFF24] = 0x77;
  p->mem[0xFF25] = 0xF3;
  p->mem[0xFF26] = 0xF1;
  p->mem[0xFF40] = 0x91;
  p->mem[0xFF42] = 0x00;
  p->mem[0xFF43] = 0x00;
  p->mem[0xFF45] = 0x00;
  p->mem[0xFF47] = 0xFC;
  p->mem[0xFF48] = 0xFF;
  p->mem[0xFF49] = 0xFF;
  p->mem[0xFF4A] = 0x00;
  p->mem[0xFF4B] = 0x00;
  p->mem[0xFFFF] = 0x00;
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
  init_proc(&p);

  // main loop
  while (1)
  {
    /*
    if (p.pc >= ...)
    {
       print_debug_info(&p);
       getchar();
    }
    */

    int res = run_operation(&p);

    if (res < 0)
    {
      printf("Stopped at pc: 0x%x\n", p.pc);
      printf("Unrecognized op: 0x%x\n", (uint8_t) memory[p.pc]);
      return -1;
    }
  }

  return 0;
}
