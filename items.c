#include <strings.h>
#include <stdio.h>

#include "data.h"
#include "util.h"

BOOL is_empty(GameState *state) {
        for (int i = 0; i<INV_SIZE; i++) {
                if (!invalid_id(state->items[i], N_ITEMS)) return FALSE;
        }
        return TRUE;
}

void list_items(GameState *state) {
        
        if (is_empty(state)) {
                printf("You don't have any items.\n");
                return;
        }

        printf("Inventory:\n");
        for (int i = 0; i<INV_SIZE; i++) {
                if (invalid_id(state->items[i], N_ITEMS)) {
                        continue;
                }
                printf("-> %s\n", items[state->items[i]-1].name);
        }
}

void add_item(GameState *state, uint8_t item_to_add) {
        if (invalid_id(item_to_add, N_ITEMS)) return;
        for (int i = 0; i<INV_SIZE; i++) {
                if (invalid_id(state->items[i], N_ITEMS)) {
                        state->items[i] = item_to_add;
                        if (items[item_to_add - 1].pickup) {
                                items[item_to_add - 1].pickup(state);
                        } else
                               printf("Picked up a %s\n", items[item_to_add-1].name);
                        return;
                }
        }
}

BOOL has_item(GameState *state, uint8_t item) {
        if (!item) return TRUE;
        for (int i = 0; i<INV_SIZE; i++) {
                if (state->items[i] == item) {
                        return TRUE;
                }
        }
        return FALSE;
}

GAction use_item(GameState *state, const char* item_name) {
        for (int i = 0; i<INV_SIZE; i++) {
                if (invalid_id(state->items[i], N_ITEMS))
                        continue;
                if (strcasecmp(items[state->items[i]-1].name, item_name)==0) {
                        // found it
                        uint8_t result = 0;
                        if (items[state->items[i]-1].callback) {
                                printf("You used %s!\n", item_name);
                                result = items[state->items[i]-1].callback(state);
                        } else {
                                printf("You can't use that!\n");
                                return GA_NOP;
                        }
                        if (items[state->items[i]-1].signle_use) {
                                state->items[i] = 0;
                        }
                        return result;
                }
        }
        // not found
        printf("You don't have any of that in your bag!\n");
        return GA_NOP;
}


/*********
 * ITEMS *
 *********/

GAction use_potion(GameState *state) {
        uint8_t old_health = state->health;
        set_health(state, state->health + (uint8_t)5);
        uint8_t healed = state->health - old_health;
        printf("Healed \e[4m%d points! (%d/%d)\e[0m\n", healed, state->health, state->max_health);
        return GA_NOP;
}

GAction equip_copper_sword(GameState *state) {
        state->atk += 3;
        printf("Equiped \e[4mcopper\e[0m sword! ATK+3\n");
        return GA_NOP;
}

const Item items[] = {
        {"Key", 1, NULL, NULL},
        {"Potion", 1, use_potion,NULL},
        {"Copper Sword", 1, NULL, equip_copper_sword},
        {"Alarm clock", 0, use_alarm, NULL},
};
const int N_ITEMS = (sizeof(items)/sizeof(items[0]));
