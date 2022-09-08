#include "data.h"
#include "util.h"
#include <stdio.h>
#include <unistd.h>

uint8_t process_battle(GameState *state, uint8_t battle_id) {
        if (invalid_id(battle_id, N_BATTLES)) return 0;
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
                        printf("%s hits %s! \e[4m-%d HP\e[0m\n", state->name, battle->enemy_name, player_damage);
                        enemy_hp = sub_or_zero(enemy_hp, player_damage);
                } else {
                        printf("%s hits %s! \e[4m-%d HP\e[0m\n", battle->enemy_name, state->name, enemy_damage);
                        state->health = sub_or_zero(state->health, enemy_damage);
                }
                turn = 1 - turn;
                usleep(500000);
        }
        if (state->health == 0) {
                printf("%s lost the battle!\n\e[5mGAME OVER\e[0m\n", state->name);
                return 1;
        }
        printf("%s defeated %s\n", state->name, battle->enemy_name);
        add_xp(state, battle->xp);
        add_item(state, battle->loot_item);
        return 0;
}


/***********
 * BATTLES *
 ***********/

const Battle battles[] = {
        {"Guard", 20, 2, 1, 5, 2},
        {"Strong guard", 40, 5, 0, 10, 0},
};
const int N_BATTLES = (sizeof(battles)/sizeof(battles[0]));
