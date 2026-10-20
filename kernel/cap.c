#include "eog.h"

static cap_token tokens[EOG_MAX_CAPS];
static uint32_t  next_id = 1;

cap_token* cap_grant(uint32_t owner, uint32_t rights, uint32_t ttl) {
    for (int i = 0; i < EOG_MAX_CAPS; i++) {
        if (tokens[i].id == 0) {
            tokens[i].id          = next_id++;
            tokens[i].rights      = rights;
            tokens[i].owner_pid   = owner;
            tokens[i].expiry_tick = ttl;
            ledger_write("CAP_GRANT", owner, tokens[i].id);
            return &tokens[i];
        }
    }
    return 0;
}

int cap_check(uint32_t token_id, uint32_t required) {
    for (int i = 0; i < EOG_MAX_CAPS; i++)
        if (tokens[i].id == token_id)
            return (tokens[i].rights & required) == required;
    return 0;
}

void cap_revoke(uint32_t token_id) {
    for (int i = 0; i < EOG_MAX_CAPS; i++) {
        if (tokens[i].id == token_id) {
            ledger_write("CAP_REVOKE", tokens[i].owner_pid, token_id);
            tokens[i].id = 0;
            return;
        }
    }
}
