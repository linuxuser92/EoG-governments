#include "eog.h"

extern void gdt_init(void);
extern void idt_init(void);

void kmain(uint32_t magic, uint32_t mb_info) {
    (void)magic;
    (void)mb_info;

    gdt_init();
    idt_init();

    puts("=========================================\n");
    puts("  Everyone of Government (EoG) v0.1\n");
    puts("  Sovereign Core initializing...\n");
    puts("=========================================\n");

    ledger_write("BOOT", magic, mb_info);

    puts("[Constitution] GDT loaded\n");
    puts("[Ballot] IDT loaded\n");

    cap_token* t = cap_grant(0, CAP_ALL, 0);
    puts("[Citizenship] root token granted id=");
    puthex(t->id);
    putc('\n');

    puts("[Ledger] boot recorded\n");
    puts("\nEoG ready. Awaiting agencies (drivers).\n");

    for (;;) __asm__ volatile ("hlt");
}
