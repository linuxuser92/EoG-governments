#include <stdint.h>
#include "eog.h"

static volatile uint16_t* vga = (uint16_t*)0xB8000;
static int row = 0, col = 0;

void putc(char c) {
    if (c == '\n') { col = 0; row++; }
    else {
        vga[row * 80 + col++] = (uint16_t)(uint8_t)c | (0x07 << 8);
        if (col >= 80) { col = 0; row++; }
    }
    if (row >= 25) {
        /* crude scroll: shift all lines up, blank the last */
        for (int r = 1; r < 25; r++)
            for (int c = 0; c < 80; c++)
                vga[(r-1)*80 + c] = vga[r*80 + c];
        for (int c = 0; c < 80; c++)
            vga[24*80 + c] = (0x07 << 8) | ' ';
        row = 24;
    }
}

void puts(const char* s) {
    while (*s) putc(*s++);
}

void puthex(uint32_t v) {
    char buf[8];
    for (int i = 7; i >= 0; i--) {
        uint8_t n = (v >> (i * 4)) & 0xF;
        buf[7 - i] = (char)(n < 10 ? '0' + n : 'a' + (n - 10));
    }
    puts("0x");
    for (int i = 0; i < 8; i++) putc(buf[i]);
}
