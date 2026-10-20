#include "eog.h"

static task tasks[EOG_MAX_TASKS];
static int  ntasks  = 0;
static int  current = 0;

void sched_add(uint32_t pid, uint32_t eip) {
    if (ntasks >= EOG_MAX_TASKS) return;
    tasks[ntasks].pid        = pid;
    tasks[ntasks].esp        = 0;
    tasks[ntasks].eip        = eip;
    tasks[ntasks].term_ticks = 0;
    tasks[ntasks].state      = 1;      /* ready */
    ntasks++;
    ledger_write("TASK_ENROLL", pid, 0);
}

void sched_tick(void) {
    if (ntasks == 0) return;
    tasks[current].term_ticks++;
    if (tasks[current].term_ticks >= EOG_TERM_LIMIT) {
        tasks[current].term_ticks = 0;
        current = (current + 1) % ntasks;
        ledger_write("TERM_END", tasks[current].pid, 0);
        /* real context switch goes here in a later milestone */
    }
}

int sched_current(void) {
    if (ntasks == 0) return -1;
    return current;
}
