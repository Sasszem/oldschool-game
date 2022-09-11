#include "data.h"
#include "util.h"
#include <stdio.h>
#include <unistd.h>

GAction process_battle(GameState *state, uint8_t battle_id) {
        if (invalid_id(battle_id, N_BATTLES)) return GA_NOP;
        const Battle *battle = &battles[battle_id-1];

        if (battle->autostart) {
                printf("A %s attacks %s!\n", battle->enemy_name, state->name);
        } else {
                printf("%s attacks the %s...\n", state->name, battle->enemy_name);
        }
        uint8_t turn = battle->autostart == 0; // 1 if player 0 if enemy
        uint8_t enemy_hp = battle->enemy_health;
        uint8_t enemy_invincible = enemy_hp == 0;
        uint8_t player_damage = enemy_invincible ? 0 : state->atk;
        uint8_t enemy_damage = sub_or_zero(battle->enemy_atk, state->armor) + enemy_invincible;
        while (state->health && (enemy_hp || enemy_invincible)) {
                if (turn) {
                        printf("-> %s hits %s! \e[4m-%d HP\e[0m\n", state->name, battle->enemy_name, player_damage);
                        enemy_hp = sub_or_zero(enemy_hp, player_damage);
                } else {
                        printf("-> %s hits %s! \e[4m-%d HP\e[0m\n", battle->enemy_name, state->name, enemy_damage);
                        state->health = sub_or_zero(state->health, enemy_damage);
                }
                turn = 1 - turn;
                usleep(500000);
        }
        if (state->health == 0) {
                printf("%s lost the battle!\n\e[5mGAME OVER\e[0m\n", state->name);
                return GA_GAMEOVER;
        }
        printf("%s defeated %s\n", state->name, battle->enemy_name);
        add_xp(state, battle->xp);
        add_item(state, battle->loot_item);
        printf("\n");
        return GA_NOP;
}


/***********
 * BATTLES *
 ***********/

// name health atk auto xp item

const Battle battles[] = {
        {"Tomb guard", 15, 2, 0, 3, 1},
        {"Zombie", 30, 4, 0, 8, 0},
        {"Skeleton warrior", 60, 4, 0, 20, 7},
        {"Tomb guard", 15, 2, 0, 3, 0},
        {"Skeleton warrior", 60, 4, 0, 20, 0},
        {"Robot", 0, 100, 1, 0, 11},
        {"Vampire", 120, 120, 1, 0, 0},
        {"Renfield", 100, 1, 1, 1, 0},
};
const int N_BATTLES = (sizeof(battles)/sizeof(battles[0]));
