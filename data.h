#pragma once

#include <stdint.h>
#include "gamestate.h"
#include "items.h"
#include "doors.h"
#include "battles.h"
#include "progress.h"
/**********
 * LEVELS *
 **********/
#define DOORS_PER_LEVEL 8
#define ITEMS_PER_LEVEL 8

typedef struct level {
        char *name;
        uint8_t battle_id;
        uint8_t doors[DOORS_PER_LEVEL];
        uint8_t items[ITEMS_PER_LEVEL];
} Level;
extern const Level levels[];
extern const int N_LEVELS;



void set_health(GameState *state, uint8_t health);
void add_xp(GameState *state, uint8_t xp);
