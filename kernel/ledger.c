#include "eog.h"

typedef struct {
    char     evt[16];
    uint32_t a;
    uint32_t b;
    uint32_t tick;
} ledger_entry;

static ledger_entry ledger[EOG_LEDGER_ENTRIES];
static uint32_t     head = 0;

void ledger_write(const char* evt, uint32_t a, uint32_t b) {
    int i = 0;
    while (evt[i] && i < 15) { ledger[head].evt[i] = evt[i]; i++; }
    ledger[head].evt[i] = 0;
    ledger[head].a    = a;
    ledger[head].b    = b;
    ledger[head].tick = timer_ticks();
    head = (head + 1) % EOG_LEDGER_ENTRIES;
}
