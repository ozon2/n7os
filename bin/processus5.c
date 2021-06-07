#include <stdio.h>
#include <n7OS/proc.h>

void processus5() {
  for (int i = 0;;i++) {
    printf("Hello, world from P5: %i\n", i);
    for (int i = 0; i < 100000000; i++);
    schedule();
  }
}
