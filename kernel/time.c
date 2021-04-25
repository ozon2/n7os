#include <debug.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>

#define PIC_COMMAND_PORT 0x20
#define PIC_CONFIG_PORT 0x21
#define IRQ_TIMER_PORT 0
#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43
uint16_t FREQUENCE = 1190; // f_osc/horloge avec f_osc = 1.19 Mhz et horloge = 1 kHz 

int time = 0;

// IT 32 : timer
extern void handler_IT_32();

void enable_IT_timer() {
    // Démasquage de l'IT du timer
    outb(inb(PIC_CONFIG_PORT)&~(1<<IRQ_TIMER_PORT), PIC_CONFIG_PORT);
}

void disable_IT_timer() {
    // Masquage de l'IT du timer
    outb(inb(PIC_CONFIG_PORT)|(1<<IRQ_TIMER_PORT), PIC_CONFIG_PORT);
}

void init_it_32() {
    init_irq_entry(32, (uint32_t) handler_IT_32);
}

void gestionIT32() {
    disable_IT_timer();
    outb(0x20, PIC_COMMAND_PORT); // acquittement de l'interruption
    if (time % 100 == 0) {
        printf("time\n");
    }
    time++;
    enable_IT_timer();
}

void init_timer() {
    // Configuration du PIT
    outb(0b00110100, PIT_COMMAND_PORT); // Channel 0, accès poids faible/poids fort, générateur d’impulsion, fréquence définie en binaire
    outb(FREQUENCE&0xFF, PIT_CHANNEL0_PORT); // Affectation de la fréquence, poids faible, au Channel 0
    outb(FREQUENCE>>8, 0x40); // Affectation de la fréquence, poids faible, au Channel 0

    // Activation de l’IT du timer
    init_it_32();
    enable_IT_timer();

    // printf("Timer : %i s\n", time);
}
