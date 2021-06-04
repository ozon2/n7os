#include <stdio.h>
#include <n7OS/proc.h>

void processus1() {
  for (int i = 0;;i++) {
    printf("Hello, world from P1: %i\n", i);
    schedule();
  }
}
