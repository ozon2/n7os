#include <stdio.h>
#include <n7OS/proc.h>

void processus3() {
  for (int i = 0;;i++) {
    printf("Hello, world from P3: %i\n", i);
    for (int i = 0; i < 100000000; i++);
    schedule();
  }
}
