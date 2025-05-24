#include "func8-10.h"
#include <stdio.h>

int main() {
  int x = 10, y = 10;

  printf("Пример прямой рекурсии(сумма чисел от 0 до n):\n");
  int answer = sum(x);
  printf("Сумма чисел от 0 до %d равна %d\n", x, answer);

  printf("Пример косвенной рекурсии:\n");
  funcA(y);
  asm("nop");

  return 0;
}
  

  
  
