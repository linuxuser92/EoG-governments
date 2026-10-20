#include "eog.h"

static volatile uint32_t ticks = 0;

uint32_t timer_ticks(void) { return ticks; }

void isr_handler(void) {
    ledger_write("EXCEPTION", 0, 0);
    for (;;) __asm__ volatile ("hlt");
}

void timer_handler(void) {
    ticks++;
    outb(0x20, 0x20);          /* EOI to master PIC */
    sched_tick();
}
