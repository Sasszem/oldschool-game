#include <strings.h>
#include "data.h"
#include "util.h"
#include "string.h"

#include <stdio.h>

void list_items(GameState *state) {
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

uint8_t has_item(GameState *state, uint8_t item) {
        if (!item) return 1;
        for (int i = 0; i<INV_SIZE; i++) {
                if (state->items[i] == item) {
                        return 1;
                }
        }
        return 0;
}

void use_item(GameState *state, const char* item_name) {
        for (int i = 0; i<INV_SIZE; i++) {
                if (invalid_id(state->items[i], N_ITEMS))
                        continue;
                if (strcasecmp(items[state->items[i]-1].name, item_name)==0) {
                        // found it
                        if (items[state->items[i]-1].callback) {
                                printf("You used %s!\n", item_name);
                                items[state->items[i]-1].callback(state);
                        } else {
                                printf("You can't use that!\n");
                                return;
                        }
                        if (items[state->items[i]-1].signle_use) {
                                state->items[i] = 0;
                        }
                        return;
                }
        }
        // not found
        printf("You don't have any of that in your bag!\n");
}


/*********
 * ITEMS *
 *********/

uint8_t use_potion(GameState *state) {
        uint8_t old_health = state->health;
        set_health(state, state->health + (uint8_t)5);
        uint8_t healed = state->health - old_health;
        printf("Healed \e[4m%d points! (%d/%d)\e[0m\n", healed, state->health, state->max_health);
        return 0;
}

uint8_t equip_copper_sword(GameState *state) {
        state->atk += 3;
        printf("Equiped \e[4mcopper\e[0m sword! ATK+3\n");
        return 0;
}

const Item items[] = {
        {"Key", 1, NULL, NULL},
        {"Potion", 1, use_potion,NULL},
        {"Copper Sword", 1, NULL, equip_copper_sword},
};
const int N_ITEMS = (sizeof(items)/sizeof(items[0]));
