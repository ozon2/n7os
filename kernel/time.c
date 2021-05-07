#include <debug.h>
#include <inttypes.h>
#include <stdbool.h>
#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
#include <n7OS/time.h>

#define PIC_COMMAND_PORT 0x20
#define PIC_CONFIG_PORT 0x21
#define IRQ_TIMER_PORT 0
#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43

uint16_t FREQUENCE = 1190; // f_osc/horloge avec f_osc = 1.19 Mhz et horloge = 1 kHz 
Time timer; // Temps système

// IT 32 : timer
extern void handler_IT_32();

void enable_IT_timer() {
    // Démasquage de l'IT du timer
    outb(inb(PIC_CONFIG_PORT)&~(1<<IRQ_TIMER_PORT), PIC_CONFIG_PORT);
}

void init_it_32() {
    init_irq_entry(32, (uint32_t) handler_IT_32);
}

// Incrémente le timer toute les millisecondes
void update_timer() {
    bool show = false; // Est-ce qu'on doit mettre à jour l'affichage ?
    timer.msec++;
    if (timer.msec == 1000) {
        timer.msec = 0;
        timer.sec++;
        show = true;
    }
    if (timer.sec == 60) {
        timer.sec = 0;
        timer.min++; 
    } 
    if (timer.min == 60) {
        timer.min = 0;
        timer.hour++;
    }
    if (show) {
        // TODO:
        // snprintf
        // strcat
        // console_putchar_at()
    }
}

void gestionIT32() {
    update_timer();
    outb(0x20, PIC_COMMAND_PORT); // acquittement de l'interruption
}

void init_timer() {
    // Initialiser le timer
    timer.sec = 0;
    timer.min = 0;
    timer.hour = 0;

    // Activation de l’IT du timer
    init_it_32();
    enable_IT_timer();

    // Configuration du PIT
    outb(0b00110100, PIT_COMMAND_PORT); // Channel 0, accès poids faible/poids fort, générateur d’impulsion, fréquence définie en binaire
    outb(FREQUENCE&0xFF, PIT_CHANNEL0_PORT); // Affectation de la fréquence, poids faible, au Channel 0
    outb(FREQUENCE>>8, 0x40); // Affectation de la fréquence, poids faible, au Channel 0
}
