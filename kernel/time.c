#include <debug.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/irq.h>

#define PIC_COMMAND_PORT 0x20
#define PIC_CONFIG_PORT 0x21
#define IRQ_TIMER_PORT 0

int time = 0;

// IT 32 : timer
extern void handler_IT_32();

void init_it_32() {
    init_irq_entry(32, (uint32_t) handler_IT_32);
}

void gestionIT32() {
    printf("Interruption timer %i!\n", time);
    time++;
    outb(0x20, PIC_COMMAND_PORT); // acquittement de l'interruption
}

void init_timer() {
    outb(inb(PIC_CONFIG_PORT)&~(1<<IRQ_TIMER_PORT), PIC_CONFIG_PORT); // activation de l’IT du timer
    // outb(inb(0x21)&0xfe, 0x21);
    init_it_32();
}
