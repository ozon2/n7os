#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>


void fibonacci() {
  int i, nb1, nb2, res, n;
  n=5;
  for (i=0;i<n;i++) {
    if (i <= 1) {
      res = i;
    } else {
      res = nb1 + nb2;
      nb1 = nb2;
      nb2 = res;
    }
  }
}


void kernel_start(void)
{
    // fibonacci();
    
    // on ne doit jamais sortir de kernel_start
    while (1) {
        console_init();  
        console_putbytes("Hello World! ", 12);
        // cette fonction arrete le processeur
        hlt();
    }
}
