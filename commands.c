#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "data.h"
#include "util.h"
#include "progress.h"
#include "save.h"

typedef GAction (*command_callback)(GameState*, const char*);

typedef struct cmd {
        const char *name;
        const char *desc;
        command_callback cmd;
        uint8_t hidden;
} Command;
Command commands[];
const int N_CMDS;

GAction loot(GameState *state, const char* _) {
        if (looted_level(state, state->current_level)) goto not_found;
        if (invalid_id(state->current_level, N_LEVELS)) return GA_NOP;

        const Level *level = &levels[state->current_level - 1];
        BOOL found = FALSE;
        for (int i = 0; i<ITEMS_PER_LEVEL; i++) {
                if (invalid_id(level->items[i], N_ITEMS)) continue;
                add_item(state, level->items[i]);
                found = TRUE;
        }
        add_looted_level(state, state->current_level);
        if (!found)
                goto not_found;
        return GA_NOP;
        not_found:
        printf("You did not find anything!\n");
        return GA_NOP;
}

GAction go_to(GameState *state, const char* where) {
        if (invalid_id(state->current_level, N_LEVELS)) return GA_NOP;


        const Level *level = &levels[state->current_level - 1];
        for (int i = 0; i<DOORS_PER_LEVEL; i++) {
                if (invalid_id(level->doors[i], N_DOORS)) continue;
                if (strcasecmp(door_name(level->doors[i]), where) == 0) {
                        // found it!
                        return process_door(state, level->doors[i]);
                }
        }
        printf("Can't go to '%s'!\n", where);
        return GA_NOP;
}

GAction attack(GameState *state, const char* _) {
        if (invalid_id(state->current_level, N_LEVELS)) return GA_NOP;
        if (cleared_level(state, state->current_level)) goto no_enemy;
        const Level *level = &levels[state->current_level - 1];
        add_cleared_level(state, state->current_level);
        if (invalid_id(level->battle_id, N_BATTLES)) goto no_enemy;
        return process_battle(state, level->battle_id);

        no_enemy:
                printf("No enemy to attack!\n");
                return GA_NOP;
}

GAction stats(GameState *state, const char* _) {
        print_room_info(state);
        printf("\n");
        list_items(state);
        printf("\n");
        printf("Stats of \e[4m%s\e[0m:\e[1m\n- HP: (%d)/(%d)\n- ATK: %d\n- ARMOR: %d\e[0m\n", state->name, state->health, state->max_health, state->atk, state->atk);
        return GA_NOP;
}


GAction help(GameState *state, const char* _) {
        printf("Alaviable commands: \n");
        for (int i = 0; i<N_CMDS; i++) {
                if (!commands[i].hidden)
                        printf("-> %s - %s\n", commands[i].name, commands[i].desc);
        }
        return GA_NOP;
}

GAction use_item_cmd(GameState *state, const char* item) {
        return use_item(state, item);
}

GAction quit(GameState *state, const char* item) {
        clrscr();
        printf("\e[7mThank you for playing\e[0m\n\n");
        printf("Your savestate: \e[4m");
        save_state(state);
        printf("\e[0m\n");
        printf("\nCopy & save this!\n");
        printf("\n\e[7mPRESS ANY KEY TO QUIT TO THE MENU!\e[27m\n");
        hide_cursor();
        getch();
        show_cursor();
        return GA_GAMEOVER;
}

GAction load_game(GameState *state, const char* item) {
        load_state(state, item);
        switch_level(state, state->current_level, 0);
        return stats(state, NULL);
}

GAction get_flag(GameState *state, const char* item) {
        //Test if flag file is present.
        if (access("flag", F_OK) != 0) {
                printf("Missing flag file.\n");
                return;
        }
        clrscr();
        const char* f = "       ";
        const char* form = " |\e[%dm%s\e[49m\n";
        printf("Here, enjoy your flag: SECURITEAM{\n");
        printf(form, 41, f);
        printf(form, 47, f);
        printf(form, 42, f);
        printf(" |\n |\n-+-\n}\n");
        hide_cursor();
        printf("\n\n\e[7mPRESS ANY KEY TO QUIT TO THE MENU!\e[27m");
        getch();
        return GA_GAMEOVER;
}

GAction debug_dump(GameState*state, const char* item) {
        printf("Debug dump: \n");
        printf("- Level = %d\n", state->current_level);
        printf("- XP = %d\n",state->xp);
        printf("- looted_levels: \n");
        for (int i = 0; i<STORE; i++) {
                if (state->looted_levels[i]) printf("--- %d\n", state->looted_levels[i]);
        }
        printf("- cleared_rooms: \n");
        for (int i = 0; i<STORE; i++) {
                if (state->cleared_levels[i]) printf("--- %d\n", state->cleared_levels[i]);
        }
        printf("- cleared_rooms: \n");
        for (int i = 0; i<STORE; i++) {
                if (state->opened_doors[i]) printf("--- %d\n", state->opened_doors[i]);
        }
        return GA_NOP;
}

GAction cheat_code(GameState *state, const char* _) {
        if (strcmp(state->name, "Doomguy")) {
                return GA_GAMEOVER;
        }
        state->health = state->max_health = 100;
        state->armor = 100;
        state->atk = 100;
        return stats(state, NULL);
}


Command commands[] = {
        {"ATTACK", "Attack the enemy in the room", attack, 0},
        {"A", "Attack the enemy in the room", attack, 1},
        {"?", "Print stats", stats, 0},
        {"HELP", "Print help", help, 0},
        {"H", "Print help", help, 1},
        {"LOOT", "Search for items in the room", loot, 0},
        {"L", "Search for items in the room", loot, 1},
        {"GO TO", "Go to location", go_to, 0},
        {"USE", "Use item", use_item_cmd, 0},
        {"U", "Use item", use_item_cmd, 1},
        {"QUIT", "Quit game (with saving)", quit, 0},
        {"Q", "Quit game (with saving)", quit, 1},
        {"CD", "Cd into another directory", go_to, 1},
        {"FLAG", "Get the flag and quit", get_flag, 1},
        //{"DD", "Debug dump", debug_dump, 1}, // TODO: comment this line to disable feature
        {"IDDQD", "IDDQD", cheat_code, 1}, // TODO: comment this line to disable feature
};

const int N_CMDS = sizeof(commands) / sizeof(commands[0]);


GAction run_command(GameState *state, const char* command) {
        if (command[0] == 0) return GA_NOP;

        for (int i = 0; i<N_CMDS; i++) {
                if (strncasecmp(commands[i].name, command, strlen(commands[i].name))==0 && (command[strlen(commands[i].name)] == ' ' || command[strlen(commands[i].name)] == '\0')) {
                        return commands[i].cmd(state, command + strlen(commands[i].name)+1);
                }
        }
        printf("Invalid command: '%s'\n", command);
        return GA_NOP;
}