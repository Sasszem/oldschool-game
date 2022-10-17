#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "data.h"
#include "util.h"
#include "commands.h"
#include "enums.h"
#include "save.h"
// todo: xor stream crypto
// todo: game state
// todo: game

char* help_text = 
        "Welcome to the CURSED GRAVEYARD game!\n\n"
        "\e[5m\e[7m\e[31mCONGRATULATIONS\e[27m\e[39m\e[25m for purchasing one of the best singleplayer adventure games of this century!\n"
        "Your goal is simple: defeat the EVIL VAMPIRE that keeps the whole country in constant fear.\n"
        "To do that, you have to enter his tomb, fight your way trough the dungeon, defeat all enemies, level up your skills, collect items and find where the evil vampire is hiding.\n"
        "\n"
        "\e[7mMENU\e[27m\n"
        "If you are reading this help, you should already be familiar with the menu.\nWhen starting the new game, you can pick from 4 options:\n"
        "- Start a new game (N) - this will start a new game. You will get to the character creation screen.\n"
        "- Load game (L) - you have to enter a save-state into a prompt, then you can continue the game where your left.\n"
        "- Help (H) - show this help you are currently reading\n"
        "- Quit (Q) - this option quits the game\n"
        "\nYou can always return to the menu from the game via the command 'QUIT'.\n\n"
        "\e[7mGAMEPLAY\e[27m\n"
        "You control your hero by giving them commands. For a list of commands, you can use the in-game command 'HELP'. You can often use the first letter of a command as a shortcut.\n"
        "Commands: \n"
        "- HELP - prints help for commands\n"
        "- ATTACK - attack sleeping enemy in the area\n"
        "- ? - print status of your hero, including information of the current area\n"
        "- LOOT - search the current area for items\n"
        "- GO TO <position> - go to another area. Beware: the door might be locked or guarded!\n"
        "- USE <item> - use an item from your inventory.\n"
        "- QUIT - quit to the menu, saving your state\n"
        "\n\e[7mPress q to quit help!\e[27m\n";
        ;

void do_help() {
        clrscr();
        fputs(help_text, stdout);
        int c;
        while (c = getch(), c!='q' && c != 'Q');
}

BOOL do_load(GameState *state) {
        clrscr();
        printf("\e[7mLOAD GAME\e[27m\n\n");
        printf("Enter your save bellow, then press Button to load it!\n\n> ");
        
        char* line = NULL;
        size_t len = 0;
        ssize_t retcode = getline(&line, &len, stdin);
        if (retcode < 0) {
                return FALSE;
        }

        if (load_state(state, line)) {
                free(line);
                return TRUE;
        }
        printf("\n\e[7mPRESS ANY KEY TO GO BACK TO MAIN MENU\e[27m\n");
        free(line);
        hide_cursor();
        getch();
        show_cursor();
        return FALSE;
}

void print_menu() {
        clrscr();
        printf("\e[31m\e[5m");

        puts("▄█▄      ▄   █▄▄▄▄   ▄▄▄▄▄   ▄███▄   ██▄         ▄▀  █▄▄▄▄ ██       ▄   ▄███▄ ▀▄    ▄ ██   █▄▄▄▄ ██▄       ");
        puts("█▀ ▀▄     █  █  ▄▀  █     ▀▄ █▀   ▀  █  █      ▄▀    █  ▄▀ █ █       █  █▀   ▀  █  █  █ █  █  ▄▀ █  █      ");
        puts("█   ▀  █   █ █▀▀▌ ▄  ▀▀▀▀▄   ██▄▄    █   █     █ ▀▄  █▀▀▌  █▄▄█ █     █ ██▄▄     ▀█   █▄▄█ █▀▀▌  █   █     ");
        puts("█▄  ▄▀ █   █ █  █  ▀▄▄▄▄▀    █▄   ▄▀ █  █      █   █ █  █  █  █  █    █ █▄   ▄▀  █    █  █ █  █  █  █      ");
        puts("▀███▀  █▄ ▄█   █             ▀███▀   ███▀       ███    █      █   █  █  ▀███▀  ▄▀        █   █   ███▀      ");
        puts("        ▀▀▀   ▀                                       ▀      █     █▐                   █   ▀              ");
        puts("                                                            ▀      ▐                   ▀                   ");
        printf("\e[39m\e[0m");


        printf("Welcome to hell, my friend!\n\n");

        
        printf("\e[31m");
        puts("              ");
        puts(" /|/| _     . ");
        puts("/   |(-/)(/ . ");
        puts("              ");
        printf("\e[39m");
        puts("- Press \e[7mS\e[27mTART to start a new game!");
        puts("- Press \e[7mL\e[27mOAD to load saved game!");
        puts("- Press \e[7mH\e[27mELP to open help!");
        puts("- Press \e[7mQ\e[27mUIT to quit the game!");
        puts("\n");
}

void play(GameState *state) {        
        printf("\e[0;0H\e[2J");
        char*line = NULL;
        size_t len = 0;
        ssize_t  retcode;
        run_command(state, "?");
        GAction gstate = switch_level(state, state->current_level, 1);
        //uint8_t gstate = 0;
        while (gstate != GA_GAMEOVER) {

                len = 0;
                line = NULL;
                printf("> ");
                retcode = getline(&line, &len, stdin);
                if (retcode < 0) {
                        gstate = run_command(state, "quit");
                } else if (retcode == 0) {
                        continue;
                } else {
                        line[retcode -1] = '\0';
                        
                        gstate = run_command(state, line);
                        free(line);
                }
        }
        hide_cursor();
        getch();
        show_cursor();
}



void start_new_game(GameState *state) {
        restart:
        clrscr();
        // setup base state
        hide_cursor();
        printf("What is your cast, hero?\n");
        printf("- \e[7mK\e[27mnight (+3 ARMOR)\n");
        printf("- \e[7mM\e[27masochist (+6 HP)\n");
        printf("- \e[7mW\e[27marrior (+4 ATK)\n");
        printf("\n");
        reset_state(state);

        char_select:
        switch (getch()) {
                case 'K': case 'k':
                        // knight
                        state->armor += 3;
                        printf("A safe player, I see\n");
                        break;
                case 'M': case 'm': case 'd': case 'D':
                        // darkness
                        state->max_health = state->health = state->max_health + 6;
                        printf("Now you are not invincible but you are happy with it!\n");
                        break;
                case 'W': case 'w':
                        // warrior
                        state->atk += 4;
                        printf("Quite a strong choice, have some mercy!\n");
                        break;
                default:
                        goto char_select;
        }
        show_cursor();
        printf("\nWhat is your name, hero? [30 characters]\n");
        scanf("%31s", state->name);
        while (getchar() != '\n');

        hide_cursor();
        printf("\nAre you ready for your great adventure, %s?\n", state->name);
        printf("\e[7mY\e[27mES or \e[7mR\e[27mESTART\n");

        wait_for_exit:
        switch(getch()) {
                case 'y': case 'Y':
                        break;
                case 'r': case 'R': goto restart;
                default:
                goto wait_for_exit;
        }
        show_cursor();
        return;
}

void restore_term() {
        static int done;
        if (done) return;
        done = 1;
        printf("\e[?1049lThank you for playing Wing Commander!\nSegmentation fault (core dumped)\n");
        exit(139);
}

GameState state = {{"Jauzsi!"}, 20, 20, 0, 6, 0, 0, {0}, {0}, {0},{0}};
int main(int argc, char** argv) {
        setvbuf(stdin, NULL, _IONBF, 0);
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
        printf("\e[?1049h");
        atexit(restore_term);
        signal(SIGTERM, restore_term);
        signal(SIGINT, restore_term);
        signal(SIGTSTP, restore_term);
        signal(SIGSEGV, restore_term);
        
        while (1) {
                print_menu();
                printf("\e[?25l");
                switch (getch()) {
                        case 's':
                        case 'S':
                                printf("\e[?25h");
                                start_new_game(&state);
                                play(&state);
                                break;
                        case 'L': case 'l':
                                printf("\e[?25h");
                                if (do_load(&state))
                                        play(&state);
                                break;
                        case 'H': case 'h':
                                do_help();
                                break;
                        case 'Q': case 'q':
                                goto quit;
                }
        }

        quit: return 0;
}
