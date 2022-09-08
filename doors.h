#pragma once
#include "gamestate.h"

typedef struct door {
        uint8_t target_room_id;
        uint8_t lock_item_id;
        uint8_t battle_id;
} Door;
extern const Door doors[];
extern const int N_DOORS;


uint8_t guard_state(const GameState *state, uint8_t door_id);

char* door_name(uint8_t door_id);

char* guard_name(uint8_t door_id);

uint8_t switch_level(GameState *state, uint8_t level_id);

uint8_t process_door(GameState *state, uint8_t door_id);
void print_room_info(const GameState *state);