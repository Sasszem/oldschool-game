#include "gamestate.h"
#include "enums.h"

BOOL defeated_door(const GameState *state, uint8_t door);
void add_defeated_door(GameState *state, uint8_t door);

BOOL looted_level(const GameState *state, uint8_t level);
void add_looted_level(GameState *state, uint8_t level);


BOOL cleared_level(const GameState *state, uint8_t level);
void add_cleared_level(GameState *state, uint8_t level);