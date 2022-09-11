#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "gamestate.h"
#include "save.h"

GameState state;

void encrypt_file(char* fname) {
    memset(&state, 0, sizeof(GameState));
    FILE* file;
    file = fopen(fname, "rb");
    fread(&state, sizeof(state), 1, file);
    fclose(file);
    save_state(&state);
    printf("\n");
}

void decrypt_file(char* token, char* fname) {
    load_state(&state, token);
    FILE* file;
    file = fopen(fname, "wb");
    fwrite(&state, sizeof(state), 1, file);
    fclose(file);
}

int main(int argc, char**argv) {
    if (argc < 3) {
        print_help:
        printf("Usage: %s d <token> <file> | %s e <file>\n", argv[0], argv[0]);
        return -1;
        // 5bcdd96d4a30d89dd43f8be083481f5afad6d86f41633e7e47dfd2e54e6e5bf14b714e59b97d27fb4e3462d283862e7a5d0fe592612b19b00beb955959f04ba46199fe319e47b5ee8319c00a98eb85f3f3718d599cae04ac9aab0dec924f91f3e88f2586d6da745af335678927ef8c115c1374fcbc81a64f22aa3cb4f9cda8e15ccd6832a7dc8c9a12ee2834d2a9463ac0c22f7c43d6cc6680081a7ad5c25b318fc36336a0f0d0b2def9eed2
    }
    if (argv[1][0] == 'e') {
        encrypt_file(argv[2]);
         return 0;
    } else if (argv[1][0] == 'd') {
        if (argc < 4) goto print_help;
        decrypt_file(argv[2], argv[3]);
        return 0;
    } else goto print_help;
} 