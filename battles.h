#pragma once
#include "gamestate.h"
#include "enums.h"
/***********
 * BATTLES *
 ***********/

typedef struct battle {
        char* enemy_name;
        uint8_t enemy_health;
        uint8_t enemy_atk;
        uint8_t autostart;
        uint8_t xp;
        uint8_t loot_item;
} Battle;

extern const Battle battles[];
extern const int N_BATTLES;


GAction process_battle(GameState *state, uint8_t battle_id);