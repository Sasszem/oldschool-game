#pragma once
#include <stdint.h>

/**************
 * GAME STATE *
 **************/

#define INV_SIZE 32
#define STORE 32

typedef struct game_state {
        char name[32];
        uint8_t health;
        uint8_t max_health;
        uint8_t armor;
        uint8_t atk;
        uint8_t xp;
        uint8_t current_level;
        uint8_t items[INV_SIZE];
        uint8_t opened_doors[STORE];
        uint8_t looted_levels[STORE];
        uint8_t cleared_levels[STORE];
} GameState;