#include <stdio.h>
#include "constants.h"
#include "processor.h"

int main(int argc, char* argv[])
{
  processor cpu;
  LD(&cpu.AF.r8.high, 8);
  printf("Value in reg A: %d\n", cpu.AF.r8.high);
}
