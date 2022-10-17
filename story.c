#include <stdio.h>
#include <unistd.h>
#include "data.h"
#include "util.h"

uint8_t alarm_uses;

GAction on_boss_enter(GameState *state) {
        if (invalid_id(state->current_level, N_LEVELS)) return 0;
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
        for (int i = 0; i<N_LEVELS; i++) {
                if (!cleared_level(state, i)) {
                        return FALSE;
                }
        }
        return TRUE;
}

void print_flag() {
        FILE *fptr;
        // Open file
        int c;
        fptr = fopen("flag", "r");
        if (fptr == NULL) {
                printf("Missing flag file.\n");
                return;
        }
        // printf("\e[30m");
        // Read contents from file
        while ((c = fgetc(fptr)) != EOF)
                putc(c, stdout);
        // printf("\e[39m");
        fflush(stdout);
        fclose(fptr);
}

GAction use_alarm(GameState*state) {
        if (levels[state->current_level - 1].on_display != on_boss_display) {
                printf("Nothing happened.\n");
                return GA_NOP;
        }
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
                        usleep(500000);
                        printf("[Vampire] Now I am real pissed!\n");
                        usleep(500000);
                        printf("[Vampire] MINIONS! Attack!\n");
                        usleep(3000000);
                        return (process_battle(state, 4) || (sleep(2), process_battle(state, 2)) || (sleep(2), process_battle(state, 5)) || (sleep(2), process_battle(state, 8)));
                        break;
                case 6:
                        printf("[Vampire] zZz\n");
                        break;
                case 7:
                        printf("[Vampire] WHAT? You are still here?\n");
                        usleep(500000);
                        printf("[Vampire] You got my guards?!\n");
                        usleep(500000);
                        printf("[Vampire] Do I have to deal with you myself?!\n");
                        usleep(500000);
                        printf("[Vampire] Ka-");
                        fflush(stdout);
                        usleep(500000);
                        printf("me-");
                        fflush(stdout);
                        usleep(500000);
                        printf("ha-");
                        fflush(stdout);
                        usleep(500000);
                        fflush(stdout);
                        printf("me-");
                        fflush(stdout);
                        usleep(1500000);
                        printf("HA!\n");
                        if (!all_guards_dead(state)) {
                                printf("%s lost the battle!\n\e[5mGAME OVER\e[0m\n", state->name);
                                return GA_GAMEOVER;
                        }
                        usleep(3000000);
                        printf("[%s] Is something supposed to happen?\n", state->name);
                        usleep(500000);
                        printf("[Vampire] WHAT?!\n");
                        usleep(500000);
                        printf("[Vampire] Why is it not working? Do I have no minions left?!\n");
                        usleep(500000);
                        printf("[Vampire] You did a fine job. But I am still way stronger than you are!\n");
                        usleep(3000000);
                        if (process_battle(state, 7))
                                return GA_GAMEOVER;
                        printf("[Vampire] NOOOOOOOOOOO!\n");
                        usleep(500000);
                        printf("[Vampire] How could you win when I was cheating?!\n");
                        usleep(500000);
                        printf("\e[5mVICTORY!\e[0m\n");
                        printf("Insert another coin!\n");
                        print_flag();
                        return GA_GAMEOVER;
        }
        return GA_NOP;
}