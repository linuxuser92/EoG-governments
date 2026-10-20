#include <stdint.h>
#include "eog.h"

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr   idtp;

extern void idt_load(uint32_t);
extern void isr0(void);
extern void isr32(void);

static void idt_set(int i, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[i].base_lo = base & 0xFFFF;
    idt[i].base_hi = (base >> 16) & 0xFFFF;
    idt[i].sel     = sel;
    idt[i].always0 = 0;
    idt[i].flags   = flags;
}

void idt_init(void) {
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    for (int i = 0; i < 256; i++)
        idt_set(i, (uint32_t)isr0, 0x08, 0x8E);

    /* Remap PIC so IRQs land at vectors 32..47 */
    outb(0x20, 0x11); outb(0xA0, 0x11);
    outb(0x21, 0x20); outb(0xA1, 0x28);
    outb(0x21, 0x04); outb(0xA1, 0x02);
    outb(0x21, 0x01); outb(0xA1, 0x01);
    outb(0x21, 0xFE); outb(0xA1, 0xFF);   /* unmask IRQ0 only */

    /* Bind IRQ0 (PIT) to vector 32 */
    idt_set(32, (uint32_t)isr32, 0x08, 0x8E);

    /* Program PIT to 100 Hz */
    uint32_t div = 1193180 / EOG_PIT_HZ;
    outb(0x43, 0x36);
    outb(0x40, (uint8_t)(div & 0xFF));
    outb(0x40, (uint8_t)((div >> 8) & 0xFF));

    idt_load((uint32_t)&idtp);
}
