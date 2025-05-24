#include "func8-10.h"
#include <stdio.h>

int sum(int x) {
  if (x == 0) return 0;
  return x + sum(x - 1);
}

void funcA(int x) {
  if (x > 0) {
    printf("A: %d\n", x);
    funcB(x - 1);
  }
}

void funcB(int x) {
  if (x > 0) {
    printf("B: %d\n", x);
    funcA(x / 1);
  }
  asm("mov %eax, %ebx");
}
