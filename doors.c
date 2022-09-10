#include <stdio.h>
#include "data.h"
#include "util.h"



GuardState guard_state(const GameState *state, uint8_t door_id) {
        if (invalid_id(door_id, N_DOORS)) return INVALID_DOOR;

        const Door *door = &doors[door_id - 1];
        if (invalid_id(door->battle_id, N_BATTLES) || defeated_door(state, door_id)) return NO_GUARD;

        const Battle *battle = &battles[door->battle_id - 1];
        return battle->autostart ? GUARD : SLEEPING_GUARD;
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
                        case NO_GUARD:
                                // no guard
                                printf("You can GO TO \e[4m%s\e[0m.\n", door_name(level->doors[i]));
                                break;
                        case GUARD:
                                printf("You can GO TO \e[4m%s\e[0m, but it's GUARDED BY a \e[4m%s\e[0m.\n", door_name(level->doors[i]), guard_name(level->doors[i]));
                                break;
                        case SLEEPING_GUARD:
                                printf("You can GO TO \e[4m%s\e[0m, but it's GUARDED BY a SLEEPING \e[4m%s\e[0m.\n", door_name(level->doors[i]), guard_name(level->doors[i]));
                                break;
                        default:
                                break;
                }
        }
}

GAction switch_level(GameState *state, uint8_t level_id, uint8_t skip_print) {
        if (invalid_id(level_id, N_LEVELS)) return GA_NOP;

        const Level *level = &levels[level_id - 1];

        // move player
        state->current_level = level_id;

        if (!skip_print) {
                printf("You entered the \e[4m%s\e[0m\n", level->name);
                print_level_doors(state, level);
                
                if (level->on_display)
                        level->on_display(state);
        }

        if (level->on_enter) {
                GAction r = level->on_enter(state);
                if (r != GA_NOP) return r;
        }

        if (!skip_print)
                printf("\n");
        if (!invalid_id(level->battle_id, N_BATTLES) && !cleared_level(state, level_id)) {
                if (battles[level->battle_id - 1].autostart) {
                        printf("%s is SUPRISE ATTACKED by a \e[4m%s\e[0m!\n", state->name, battles[level->battle_id - 1].enemy_name);
                        add_cleared_level(state, level_id);
                        return process_battle(state, level->battle_id);
                } else {
                        if (!skip_print)
                                printf("There is a SLEEPING \e[4m%s\e[0m!\n", battles[level->battle_id - 1].enemy_name);
                }
        }
        return GA_NOP;
}

void print_room_info(const GameState *state) {
        if (invalid_id(state->current_level, N_LEVELS)) return;
        const Level *level = &levels[state->current_level - 1];

        printf("You are in the \e[4m%s\e[0m\n", level->name);

        print_level_doors(state, level);
        if (level->on_display)
                level->on_display(state);

        if (!invalid_id(level->battle_id, N_BATTLES)) {
                printf("There is a SLEEPING \e[4m%s\e[0m!\n", battles[level->battle_id - 1].enemy_name);
        }
}

GAction process_door(GameState *state, uint8_t door_id) {
        if (invalid_id(door_id, N_DOORS)) return GA_NOP;
        const Door *door = &doors[door_id - 1];
        if (door->battle_id && (!defeated_door(state, door_id)) && process_battle(state, door->battle_id))
                return GA_GAMEOVER;
        // defeated the guard, mark as defeated
        add_defeated_door(state, door_id);
        if (has_item(state, door->lock_item_id)==FALSE) {
                printf("This door is LOCKED!\n");
                return GA_NOP;
        }

        // we can go trough
        return switch_level(state, door->target_room_id, 0);
}

/*********
 * DOORS *
 *********/


const Door doors[] = {
        {2, 1, 1},
        {1, 0, 0},
        {3, 0, 0},
};

const int N_DOORS = (sizeof(doors)/sizeof(doors[0]));
