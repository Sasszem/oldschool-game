#include <stdio.h>
#include <string.h>
#include <strings.h>

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
        if (looted_level(state, state->current_level)) return GA_NOP;
        if (invalid_id(state->current_level, N_LEVELS)) return GA_NOP;

        const Level *level = &levels[state->current_level - 1];
        for (int i = 0; i<ITEMS_PER_LEVEL; i++) {
                if (invalid_id(level->items[i], N_ITEMS)) continue;
                add_item(state, level->items[i]);
        }
        add_looted_level(state, state->current_level);
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
        if (invalid_id(state->current_level, N_LEVELS)||cleared_level(state, state->current_level)) return GA_NOP;
        const Level *level = &levels[state->current_level - 1];
        add_cleared_level(state, state->current_level);
        return process_battle(state, level->battle_id);
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
        printf("Your savestate: \e[4m");
        save_state(state);
        printf("\e[0m\n");
        printf("\e[7mThank you for playing\e[0m\n");
        return GA_GAMEOVER;
}

GAction load_game(GameState *state, const char* item) {
        load_state(state, item);
        switch_level(state, state->current_level, 0);
        return stats(state, NULL);
}

GAction get_flag(GameState *state, const char* item) {
        const char* f = "       ";
        const char* form = " |\e[%dm%s\e[49m\n";
        printf("Here, enjoy your flag{\n");
        printf(form, 41, f);
        printf(form, 47, f);
        printf(form, 42, f);
        printf(" |\n |\n-+-\n}\n");
        return GA_GAMEOVER;
}

Command commands[] = {
        {"ATTACK", "Attack the enemy in the room", attack, 0},
        {"?", "Print stats", stats, 0},
        {"HELP", "Print help", help, 0},
        {"LOOT", "Search for items in the room", loot, 0},
        {"GO TO", "Go to location", go_to, 0},
        {"USE", "Use item", use_item_cmd, 0},
        {"QUIT", "Quit game (with saving)", quit, 0},
        {"LOAD", "Load game from state", load_game, 0},
        {"CD", "Cd into another directory", go_to, 1},
        {"FLAG", "Get the flag and quit", get_flag, 1},
};

const int N_CMDS = sizeof(commands) / sizeof(commands[0]);


GAction run_command(GameState *state, const char* command) {
        if (command[0] == 0) return GA_NOP;

        for (int i = 0; i<N_CMDS; i++) {
                if (strncasecmp(commands[i].name, command, strlen(commands[i].name))==0) {
                        return commands[i].cmd(state, command + strlen(commands[i].name)+1);
                }
        }
        printf("Invalid command: '%s'\n", command);
        return GA_NOP;
}