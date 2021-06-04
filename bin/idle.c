#include <stdio.h>
#include <n7OS/proc.h>

void idle() {
  for (int i = 0;;i++) {
    printf("Hello, world from idle: %i\n", i);
    schedule();
  }
}
