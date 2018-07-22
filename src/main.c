#include <stdio.h>
#include "constants.h"
#include "processor.h"

int main(int argc, char* argv[])
{
  proc p;
  LD_imm(&p.AF.r8.high, 8);
  printf("Value in reg A: %d\n", p.AF.r8.high);
}
