#pragma once
#include "data.h"

extern uint8_t alarm_uses;

GAction on_boss_enter(GameState *state);
void on_boss_display(const GameState *state);
GAction use_alarm(GameState*state);

