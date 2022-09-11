#include "data.h"
#include "util.h"
#include "story.h"

#include <stdio.h>

void set_health(GameState *state, uint8_t health) {
        state->health  = cap_max(health, state->max_health);
}





/*********-
 * LEVELS *
 **********/

#include <unistd.h>


GAction game_start_dialog(GameState *state) {
        printf("\n[Old man] So this is it.\n");
        usleep(1000000);
        printf("[Old man] This is the tomb where the vampire sleeps.\n");
        usleep(1000000);
        printf("[Old man] Beware, this is a very powerful vampire!\n");
        usleep(1000000);
        printf("[%s] No problem! No vampire can withstand my sword!\n", state->name);
        usleep(1000000);
        printf("[Old man] Good. Then as we agreed, we pay only on success?\n");
        usleep(1000000);
        printf("[%s] Just prepare the money, I'll be back tomorrow!\n", state->name);
        usleep(1000000);
        printf("[Old man] As you wish. I will also prepare a coffin too, just in case.\n");
        usleep(1000000);
        printf("[%s] Hah! No need to worry. I'll use my trusty sword to slice that vampire in half!\n", state->name);
        if (!has_item(state, 6))
                add_item(state, 6);
        return GA_NOP;
}

void graveyard_display(const GameState *state) {
        printf("The graveyard is super-scary under the moonlight at midnigt. It was a super-smart move to come here at the morning!\n");
}


void entrance_display(const GameState *state) {
        printf("A well built, but very old room. A few puddles of water indicate that the roof faied years ago.\n");
}

void storage_display(const GameState *state) {
        printf("A dusty storage room full of cobwebs. Most likely used to store cleaning equipment, but there is a big wooden chest in the middle!\n");
}

void tomb_display(const GameState *state) {
        printf("A room with a stone tomb in the middle. Fortunatelly, it's closed.\n");
}

void staircase_display(const GameState *state) {
        printf("An old stone staircase. It's in a bad shape, most stones are cracked, and it's very dark.\n");
}

void torture_chamber_display(const GameState *state) {
        printf("A blood-chilling torture chamber. You can not imagine the horros that used to be happening here!\n");
}

void prison_display(const GameState *state) {
        printf("A prison cell, with a skeleton chained to the wall. On close examination, the skeleton looks fake and made of plastic.\n");
}

void metal_display(const GameState *state) {
        printf("You hear a strange womanly voice promising you cake, but you do not believe her, as the bloody writing on the wall says 'the cake is a lie!'\n");
}

// name, battle, doors, items, on_enter on_display
const Level levels[] = {
        {"Graveyard", 1, {1}, {0}, game_start_dialog, graveyard_display},
        {"Tomb entrance", 4, {2,3,4}, {2}, NULL, entrance_display},
        {"Storage room", 0, {5}, {3, 4, 4}, NULL, storage_display},
        {"Walkway", 2, {5, 6}, {}, NULL, NULL},
        {"Tomb", 3, {5}, {}, NULL, tomb_display},
        {"Staircase", 0, {7, 8, 9}, {5, 4}, NULL, staircase_display},
        {"Empty room", 5, {6}, {10}, NULL, NULL},
        {"Torture chamber", 5, {6, 10}, {8,8}, NULL, torture_chamber_display},
        {"Prison cell", 5, {9}, {9}, NULL, prison_display},
        {"Metal room", 6, {}, {}, NULL, metal_display},
        {"???", 0, {}, {}, on_boss_enter, on_boss_display},
};

// 13f8bb27fc6f1c614ac690f75bfe66bc8bf42ccc8c82b8446daaf89bd27e83252b9983e3f09986b4a9dbb804d021c2661ced2ea56fe6e9dd90e2786260fb878c940b6f2e025ff5ac38b3b00ddc7772f6589a97cb857fab16612478c11f004db40bbde20d1cd7b9548a6a6664dfd4533b73e809f867b50ec8d9878af887d7ac92948e9fb0655804f0c267539c3ea1d9ae8ee2a7f597b5be703c4869c31f1555b4a4f464094c69f90ed04c0ffd

// 5bcdd96d4a30d89dd43f8be083481f5afad6d86f41633e7e47dfd2e54e6e5bf14b714e59b97d27fb4e3462d283862e7a5d0fe592612b19b00beb955959f04ba46199fe319e47b5ee8319c00a98eb85f3f3718d599cae04ac9aab0dec924f91f3e88f2586d6da745af335678927ef8c115c1374fcbc81a64f22aa3cb4f9cda8e15ccd6832a7dc8c9a12ee2834d2a9463ac0c22f7c43d6cc6680081a7ad5c25b318fc36336a0f0d0b2def9eed2
const int N_LEVELS = (sizeof(levels)/sizeof(levels[0]));



/*********
 * DOORS *
 *********/

// target lock battle
const Door doors[] = {
        {2, 1, 0}, // gravayard -> entrance
        {3, 5, 4}, // entrance -> storage
        {4, 0, 0}, // entrance -> walkway
        {5, 0, 2}, // entrance -> tomb
        {2, 0, 0}, // * -> entrance
        {6, 0, 2}, // walkway -> staircase
        {4, 0, 0}, // staircase -> walkway
        {7, 0, 2}, // staircase -> empty room
        {8, 0, 0}, // staircase -> torture chamber
        {9, 0, 0}, // torture -> prison
};

const int N_DOORS = (sizeof(doors)/sizeof(doors[0]));




void add_xp(GameState *state, uint8_t xp) {
        state->xp += xp;
        while (state->xp>=10) {
                state->xp -= 10;
                state->atk += 1;
                state->armor += 1;
                state->max_health += 2;
                printf("%s reached a new level! \n\e[1m- ATK: %u\n- ARMOR: %u\n- HEALTH: %u/%u\e[0m\n", state->name, state->atk, state->armor, state->health, state->max_health);
        }
}
