#include <debug.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/processor_structs.h>
#include <n7OS/time.h>


void kernel_start(void)
{
    
    // on ne doit jamais sortir de kernel_start
    while (1) {
        console_init();
        for (int i = 0; i < 30; i++) {
          printf("%i\n", i);
        }

        sti(); // activer les interruptions

        init_it_50();
        __asm__ ("int $50"::);

        init_timer();
        // __asm__ ("int $32"::);
        
        // cette fonction arrete le processeur
        hlt();
    }
}
