#include <stdio.h>
#include "data.h"
#include "util.h"

uint8_t guard_state(const GameState *state, uint8_t door_id) {
        if (invalid_id(door_id, N_DOORS)) return 0;

        const Door *door = &doors[door_id - 1];
        if (invalid_id(door->battle_id, N_BATTLES) || defeated_door(state, door_id)) return 1;

        const Battle *battle = &battles[door->battle_id - 1];
        return battle->autostart ? 2 : 3;
}

char* door_name(uint8_t door_id) {
        if (invalid_id(door_id, N_DOORS)) return "~NODOOR~";

        uint8_t level_id = doors[door_id - 1].target_room_id;
        if (invalid_id(level_id, N_LEVELS)) return "~NOLEVEL~";
        return levels[level_id - 1].name;
}

char* guard_name(uint8_t door_id) {
        if (invalid_id(door_id, N_DOORS)) return "~MISSINGDOOR~";
        const Door *door = &doors[door_id - 1];
        if (invalid_id(door->battle_id, N_BATTLES)) return "~MISSINGB~";
        return battles[door->battle_id - 1].enemy_name;
}

void print_level_doors(const GameState *state, const Level *level) {
        for (int i = 0; i<DOORS_PER_LEVEL; i++) {
                switch (guard_state(state, level->doors[i])) {
                        case 1:
                                // no guard
                                printf("You can GO TO \e[4m%s\e[0m.\n", door_name(level->doors[i]));
                                break;
                        case 2:
                                printf("You can GO TO \e[4m%s\e[0m, but it's GUARDED BY a \e[4m%s\e[0m.\n", door_name(level->doors[i]), guard_name(level->doors[i]));
                                break;
                        case 3:
                                printf("You can GO TO \e[4m%s\e[0m, but it's GUARDED BY a SLEEPING \e[4m%s\e[0m.\n", door_name(level->doors[i]), guard_name(level->doors[i]));
                                break;
                        default:
                                break;
                }
        }
}

uint8_t switch_level(GameState *state, uint8_t level_id) {
        if (invalid_id(level_id, N_LEVELS)) return 0;

        const Level *level = &levels[level_id - 1];

        // move player
        state->current_level = level_id;

        printf("You entered the \e[4m%s\e[0m\n", level->name);
        print_level_doors(state, level);

        printf("\n");
        if (!invalid_id(level->battle_id, N_BATTLES) && !cleared_level(state, level_id)) {
                if (battles[level->battle_id - 1].autostart) {
                        printf("%s is SUPRISE ATTACKED by a \e[4m%s\e[0m!\n", state->name, battles[level->battle_id - 1].enemy_name);
                        add_cleared_level(state, level_id);
                        return process_battle(state, level->battle_id);
                } else {
                        printf("There is a SLEEPING \e[4m%s\e[0m!\n", battles[level->battle_id - 1].enemy_name);
                }
        }
        return 0;
}

void print_room_info(const GameState *state) {
        if (invalid_id(state->current_level, N_LEVELS)) return;
        const Level *level = &levels[state->current_level - 1];

        printf("You are in the \e[4m%s\e[0m\n", level->name);

        print_level_doors(state, level);

        if (!invalid_id(level->battle_id, N_BATTLES)) {
                printf("There is a SLEEPING \e[4m%s\e[0m!\n", battles[level->battle_id - 1].enemy_name);
        }
}

uint8_t process_door(GameState *state, uint8_t door_id) {
        if (invalid_id(door_id, N_DOORS)) return 1;
        const Door *door = &doors[door_id - 1];
        if (door->battle_id && (!defeated_door(state, door_id)) && process_battle(state, door->battle_id))
                return 1;        
        // defeated the guard, mark as defeated
        add_defeated_door(state, door_id);
        if (!has_item(state, door->lock_item_id)) {
                printf("This door is LOCKED!\n");
        }

        // we can go trough
        return switch_level(state, door->target_room_id);
}

/*********
 * DOORS *
 *********/


const Door doors[] = {
        {2, 1, 1},
        {1, 0, 0},
};

const int N_DOORS = (sizeof(doors)/sizeof(doors[0]));
