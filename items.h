/*********
 * ITEMS *
 *********/
#pragma once

#include "gamestate.h"
#include "enums.h"

typedef GAction (*item_callback)(GameState*);

typedef struct item
{
        char* name;
        uint8_t signle_use;
        item_callback callback;
        item_callback pickup;
} Item;

extern const Item items[];
extern const int N_ITEMS;


void list_items(GameState *state);
void add_item(GameState *state, uint8_t item_to_add);
BOOL has_item(GameState *state, uint8_t item);
GAction use_item(GameState *state, const char* item_name);