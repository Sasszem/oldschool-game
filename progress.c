
#include "data.h"
#include "util.h"

BOOL defeated_door(const GameState *state, uint8_t door) {
        if (invalid_id(door, N_DOORS)) return TRUE;
        for (int i = 0; i<STORE; i++) {
                if (state->opened_doors[i]==door) {
                        return TRUE;
                }
        }
        return FALSE;
}

void add_defeated_door(GameState *state, uint8_t door) {
        if (invalid_id(door, N_DOORS)) return;
        for (int i = 0; i<STORE; i++) {
                if (state->opened_doors[i]==0 || state->opened_doors[i] > N_DOORS) {
                        state->opened_doors[i] = door;
                        return;
                }
        }
}

BOOL looted_level(const GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return TRUE;
        for (int i = 0; i<STORE; i++) {
                if (state->looted_levels[i]==level) {
                        return TRUE;
                }
        }
        return FALSE;
}

void add_looted_level(GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return;
        for (int i = 0; i<STORE; i++) {
                if (invalid_id(state->looted_levels[i], N_LEVELS)) {
                        state->looted_levels[i] = level;
                        return;
                }
        }
}


BOOL cleared_level(const GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return TRUE;
        for (int i = 0; i<STORE; i++) {
                if (state->cleared_levels[i]==level) {
                        return TRUE;
                }
        }
        return FALSE;
}

void add_cleared_level(GameState *state, uint8_t level) {
        if (invalid_id(level, N_LEVELS)) return;
        for (int i = 0; i<STORE; i++) {
                if (invalid_id(state->cleared_levels[i], N_LEVELS)) {
                        state->cleared_levels[i] = level;
                        return;
                }
        }
}