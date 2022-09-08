#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "data.h"
#include "util.h"
#include "commands.h"

// todo: xor stream crypto
// todo: game state
// todo: game


GameState state = {{"Jauzsi!"}, 20, 20, 0, 6, 0, 0, {0}, {0}, {0},{0}};
int main() {
        uint8_t stopped = switch_level(&state, 1);
        char*line = NULL;
        size_t len = 0;
        ssize_t  retcode;
        while (!stopped) {

                len = 0;
                line = NULL;
                printf("> ");
                retcode = getline(&line, &len, stdin);
                if (retcode < 0) {
                        stopped = run_command(&state, "quit");
                } else if (retcode == 0) {
                        continue;
                } else {
                        line[retcode -1] = '\0';
                        
                        stopped = run_command(&state, line);
                        free(line);
                }
        }
        return 0;
}
