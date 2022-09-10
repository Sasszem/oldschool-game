#include <stdio.h>
#include "data.h"
#include "util.h"

uint8_t alarm_uses;

GAction on_boss_enter(GameState *state) {
        if (invalid_id(state->current_level, N_LEVELS)) return 0;

        if (state->current_level != 3) return 0; // should never trigger
        alarm_uses = 0;
        return 0;
}

void on_boss_display(const GameState *state) {
        printf("There is a coffin with a vampire sleeping in it!\n");
}

BOOL all_guards_dead(const GameState *state) {
        for (int i = 0; i<N_DOORS; i++) {
                if (!defeated_door(state, i)) {
                        return FALSE;
                }
        }

        //TODO: all rooms are cleared too!
        return TRUE;
}

GAction use_alarm(GameState*state) {
        switch (++alarm_uses) {
                case 1:
                        printf("[Vampire] Zzzzzz\n");
                        break;
                case 2:
                        printf("[Vampire] Justfivemoreminutes...\n");
                        break;
                case 3:
                        printf("[Vampire] Notnowihavenoclassestoday...\n");
                        break;
                case 4:
                        printf("[Vampire] What now?! Let me sleep!\n");
                        break;
                case 5:
                        printf("[Vampire] Enough is enought! You woke me up! I could not get even 50 years of sleep, because of you!\n");
                        printf("[Vampire] Now I am real pissed!\n");
                        printf("[Vampire] MINIONS! Attack!\n");
                        return (process_battle(state, 1) || process_battle(state, 1) || process_battle(state, 2));
                        break;
                case 6:
                        printf("[Vampire] zZz\n");
                        break;
                case 7:
                        printf("[Vampire] WHAT? You are still here?\n");
                        printf("[Vampire] You got my guards?!\n");
                        printf("[Vampire] Do I have to deal with you myself?!\n");
                        printf("[Vampire] Ka-ME-Ha-Me....");
                        fflush(stdout);
                        sleep(1);
                        printf("HA!");
                        if (!all_guards_dead(state)) {
                                printf("%s lost the battle!\n\e[5mGAME OVER\e[0m\n", state->name);
                                return GA_GAMEOVER;
                        }
                        printf("[Vampire] WHAT?!\n");
                        printf("[Vampire] Why is it not working? Do I have no minions left?!\n");
                        printf("[Vampire] You did a fine job. But I am still way stronger than you are!\n");
                        return process_battle(state, 3);
        }
        return GA_NOP;
}