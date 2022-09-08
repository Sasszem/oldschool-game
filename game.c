#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "data.h"
#include "util.h"
#include "commands.h"
uint8_t hex_nibble(char val) {
        if ('0' <= val && val <= '9')
                return val - '0';
        if ('a' <= val && val <= 'f')
                return val - 'a' + 10;
        if ('A' <= val && val <= 'F')
                return val - 'A' + 10;
        return 0;
}


uint8_t hex_to_u8(char* hex) {
        if (!(hex && hex[0] && hex[1])) return 0;
        return hex_nibble((hex[0])<<4) + hex_nibble(hex[1]);
}

// todo: xor stream crypto
// todo: game state
// todo: game


/*

typedef struct game_state {
        char name[32];
        uint8_t health;
        uint8_t max_health;
        uint8_t armor;
        uint8_t atk;
        uint8_t xp;
        uint8_t current_level;
        uint8_t items[INV_SIZE];
        uint8_t opened_doors[STORE];
        uint8_t looted_levels[STORE];
        uint8_t cleared_levels[STORE];
} GameState;*/

GameState state = {{"Jauzsi!"}, 20, 20, 0, 6, 0, 0, {0}, {0}, {0},{0}};
int main() {
        uint8_t is_dead = switch_level(&state, 1);
        char*line = NULL;
        size_t len = 0;
        uint8_t stopped = 0;
        while (!stopped) {

                len = 0;
                line = NULL;
                printf("> ");
                len = getline(&line, &len, stdin);
                line[len-1] = '\0';
                
                stopped = run_command(&state, line);
                free(line);
        }
        return 0;
}
