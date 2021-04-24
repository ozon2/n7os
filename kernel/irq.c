#include <debug.h>
#include <inttypes.h>
#include <n7OS/irq.h>


void init_irq_entry(int irq_num, uint32_t addr) {
    idt_entry_t *entry = (idt_entry_t *) &(idt[irq_num]);

    entry->offset_inf = (uint16_t) (addr & 0xFFFF);
    entry->offset_sup = (uint16_t) (addr >> 16);
    entry ->sel_segment = KERNEL_CS;
    entry->zero = 0;
    
    // P=1 (entrée configurée), DPL=00 (niveau de privilège élevé), S=0 (Trap gate), Type=1111 (Trap gate 32 bits)
    entry->type_attr  = 0b10001111;
}

// IT 50
extern void handler_IT_50();

void init_it_50() {
    init_irq_entry(50, (uint32_t) handler_IT_50);
}

void gestionIT50() {
    printf("IT 50 !\n");
}
