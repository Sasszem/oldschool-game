#include "gamestate.h"

uint8_t defeated_door(const GameState *state, uint8_t door);
void add_defeated_door(GameState *state, uint8_t door);

uint8_t looted_level(const GameState *state, uint8_t level);
void add_looted_level(GameState *state, uint8_t level);


uint8_t cleared_level(const GameState *state, uint8_t level);
void add_cleared_level(GameState *state, uint8_t level);