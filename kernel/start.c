#include <debug.h>
#include <inttypes.h>
#include <unistd.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/processor_structs.h>
#include <n7OS/time.h>
#include <n7OS/sys.h>
#include <n7OS/proc.h>


void kernel_start(void)
{
    console_init(); // initialiser la console
    sti(); // activer les interruptions
    init_syscall(); // initialiser les appels système
    init_timer(); // démarrer l'affichage du timer

    for (int i = 0; i < 30; i++) {
      printf("%i\n", i);
    }

    init_it_50();
    __asm__ ("int $50"::);

    // shutdown(1);

    initProcess();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        
        // cette fonction arrete le processeur
        hlt();
    }
}
