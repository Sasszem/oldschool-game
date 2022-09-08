
#include "data.h"
#include "util.h"

uint8_t defeated_door(GameState *state, uint8_t door) {
        if (invalid_id(door, N_DOORS)) return 1;
        for (int i = 0; i<STORE; i++) {
                if (state->opened_doors[i]==door) {
                        return 1;
                }
        }
        return 0;
}

void add_defeated_door(GameState *state, uint8_t door) {
        if (invalid_id(door, N_DOORS)) return;
        for (int i = 0; i<STORE; i++) {
                if (state->opened_doors[i]==0 || state->opened_doors[i] > N_DOORS) {
                        state->opened_doors[i] = door;
                }
        }
}

uint8_t looted_level(GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return 1;
        for (int i = 0; i<STORE; i++) {
                if (state->looted_levels[i]==level) {
                        return 1;
                }
        }
        return 0;
}

void add_looted_level(GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return;
        for (int i = 0; i<STORE; i++) {
                if (invalid_id(state->looted_levels[i], N_LEVELS)) {
                        state->looted_levels[i] = level;
                }
        }
}


uint8_t cleared_level(GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return 1;
        for (int i = 0; i<STORE; i++) {
                if (state->cleared_levels[i]==level) {
                        return 1;
                }
        }
        return 0;
}

void add_cleared_level(GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return;
        for (int i = 0; i<STORE; i++) {
                if (invalid_id(state->cleared_levels[i], N_LEVELS)) {
                        state->cleared_levels[i] = level;
                }
        }
}