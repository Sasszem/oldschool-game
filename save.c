#include "save.h"

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void print_as_hex(void* data, size_t size) {
    uint8_t *data_ptr = (uint8_t*) data;
    for (size_t i = 0; i<size; i++)
        printf("%02x", data_ptr[i]);
}

void save_state(GameState *state) {
    srand(time(NULL));
    for (int i = 0; i<100; i++, rand());
    unsigned nonce = rand();
    print_as_hex(&nonce, sizeof(nonce));

    uint16_t checksum = nonce;
    uint8_t *data = (uint8_t*)state;
    srand(nonce);
    for (int i = 0; i<sizeof(GameState); i++) {
        checksum += data[i];
        printf("%02x", data[i] ^ (uint8_t)rand());
    }
    print_as_hex(&checksum, sizeof(checksum));
}

uint8_t hex_nibble(char val) {
        if ('0' <= val && val <= '9')
                return val - '0';
        if ('a' <= val && val <= 'f')
                return val - 'a' + 10;
        if ('A' <= val && val <= 'F')
                return val - 'A' + 10;
        return 0;
}


uint8_t hex_to_u8(const char* hex) {
        if (!(hex && hex[0] && hex[1])) return 0;
        return (hex_nibble(hex[0])<<4) + hex_nibble(hex[1]);
}

uint8_t load_from_hex(void* target, const char* hex, size_t size) {
    if (strlen(hex) < size * 2) return 0;

    uint8_t *data_ptr = (uint8_t*)target;
    for (size_t i = 0; i<size; i++)
        data_ptr[i] = hex_to_u8(&hex[i*2]);
    return 0;
}

void load_state(GameState *state, const char* save) {
    unsigned nonce;
    const char* save_ptr = save;
    if (load_from_hex(&nonce, save_ptr, sizeof(nonce))) goto error;
    save_ptr += sizeof(nonce) * 2;

    if (load_from_hex(state, save_ptr, sizeof(GameState))) goto error;
    save_ptr += sizeof(GameState) * 2;

    uint16_t checksum_s;
    if (load_from_hex(&checksum_s, save_ptr, sizeof(checksum_s))) goto error;

    srand(nonce);
    uint16_t checksum = nonce;
    uint8_t *data = (uint8_t *)state;
    for (int i = 0; i<sizeof(GameState); i++) {
        data[i] ^= rand();
        checksum += data[i];
    }

    if (checksum != checksum_s) {
        printf("Load failed: invalid checksum!\n");
        reset_state(state);        
    } else {
        printf("Load complete!\n");
    }
    return;

    error:
    printf("Load failed: invalid length!");
    return;
}

GameState default_state = {
        {0}, // name
        15, // HP
        15, // maxHP
        0, // ARMOR
        1, // ATK
        0, // XP
        1, // LEVEL
        {0},
        {0},
        {0},
        {0},
};

void reset_state(GameState *state) {
    memcpy(state, &default_state, sizeof(GameState));
}

