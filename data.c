#include "data.h"
#include "util.h"
#include <stdio.h>

void set_health(GameState *state, uint8_t health) {
        state->health  = cap_max(health, state->max_health);
}







/*********-
 * LEVELS *
 **********/

const Level levels[] = {
        {"First room", 2, {1}, {1,3}},
        {"Second room", 1, {}, {}},
};
const int N_LEVELS = (sizeof(levels)/sizeof(levels[0]));




void add_xp(GameState *state, uint8_t xp) {
        state->xp += xp;
        while (state->xp>=10) {
                state->xp -= 10;
                state->atk += 1;
                state->armor += 1;
                state->max_health += 2;
                printf("%s reached a new level! \n\e[1m- ATK: %u\n- ARMOR: %u\n- HEALTH: %u/%u\e[0m\n", state->name, state->atk, state->armor, state->health, state->max_health);
        }
}
