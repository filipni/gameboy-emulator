#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "processor.h"
#include "utils.h"
#include "memory.h"

#define ROM_FILE "roms/tetris.gb"
#define BOOTSTRAP_FILE "roms/DMG_ROM.bin"
#define ROM_SIZE 32768

#define MEMORY_SIZE 0x10000

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

}

int main(int argc, char* argv[])
{
  load_rom(ROM_FILE, ROM_SIZE);

  proc p;
  init_proc(&p);
  init_memory();

  // main loop
  while (1)
  {
       print_debug_info(&p);
       printf("Current op: 0x%x\n", memory[p.pc]);
       getchar();

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
