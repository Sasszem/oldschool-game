#include <string.h>
#include "data.h"
#include "util.h"
#include "progress.h"
#include <stdio.h>

typedef uint8_t (*command_callback)(GameState*, const char*);

typedef struct cmd {
        const char *name;
        const char *desc;
        command_callback cmd;
} Command;
Command commands[];
const int N_CMDS;

uint8_t loot(GameState *state, const char* _) {
        if (looted_level(state, state->current_level)) return 0;
        if (invalid_id(state->current_level, N_LEVELS)) return 0;

        const Level *level = &levels[state->current_level - 1];
        for (int i = 0; i<ITEMS_PER_LEVEL; i++) {
                if (invalid_id(level->items[i], N_ITEMS)) continue;
                add_item(state, level->items[i]);
        }
        add_looted_level(state, state->current_level);
        return 0;
}

uint8_t go_to(GameState *state, const char* where) {
        if (invalid_id(state->current_level, N_LEVELS)) return 0;


        const Level *level = &levels[state->current_level - 1];
        for (int i = 0; i<DOORS_PER_LEVEL; i++) {
                if (invalid_id(level->doors[i], N_DOORS)) continue;
                if (strcmp(door_name(level->doors[i]), where) == 0) {
                        // found it!
                        return process_door(state, level->doors[i]);
                }
        }
        printf("Can't go to '%s'!\n", where);
        return 0;
}

uint8_t attack(GameState *state, const char* _) {
        printf("CHAAARGE!\n");
        if (invalid_id(state->current_level, N_LEVELS)||cleared_level(state, state->current_level)) return 0;
        const Level *level = &levels[state->current_level - 1];
        add_cleared_level(state, state->current_level);
        return process_battle(state, level->battle_id);
}

uint8_t stats(GameState *state, const char* _) {
        printf("%s:\e[1m\n- HP: (%d)/(%d)\n- ATK: %d\n- ARMOR: %d\e[0m\n", state->name, state->health, state->max_health, state->atk, state->atk);
        return 0;
}


uint8_t help(GameState *state, const char* _) {
        printf("Alaviable commands: \n");
        for (int i = 0; i<N_CMDS; i++) {
                printf("-> %s - %s\n", commands[i].name, commands[i].desc);
        }
        return 0;
}

uint8_t list_items_cmd(GameState *state, const char* _) {
        list_items(state);
        return 0;
}

uint8_t use_item_cmd(GameState *state, const char* item) {
        use_item(state, item);
        return 0;
}

uint8_t quit(GameState *state, const char* item) {
        printf("Bye!\n");
        return 1;
}

Command commands[] = {
        {"ATTACK", "Attack the enemy in the room", attack},
        {"?", "Print stats", stats},
        {"HELP", "Print help", help},
        {"LOOT", "Search for items in the room", loot},
        {"GO TO", "Go to location", go_to},
        {"ITEMS", "List items", list_items_cmd},
        {"USE", "Use item", use_item_cmd},
        {"QUIT", "Quit game", quit},
};

const int N_CMDS = sizeof(commands) / sizeof(commands[0]);


uint8_t run_command(GameState *state, const char* command) {
        for (int i = 0; i<N_CMDS; i++) {
                if (strncmp(commands[i].name, command, strlen(commands[i].name))==0) {
                        return commands[i].cmd(state, command + strlen(commands[i].name)+1);
                }
        }
        printf("Invalid command: '%s'\n", command);
        return 0;
}