#include "gamestate.h"
#include "enums.h"

void save_state(GameState *state);
BOOL load_state(GameState *state, const char* save);
void reset_state(GameState *state);