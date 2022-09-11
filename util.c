#include "util.h"
#include "enums.h"

uint8_t sub_or_zero(uint8_t a, uint8_t b) {
        return a>b ? a-b : 0;
}

uint8_t cap_max(uint8_t x, uint8_t max) {
        return x > max ? max : x;
}

BOOL invalid_id(uint8_t id, uint8_t max) {
        return !id || id > max;
}



// getch straight from https://stackoverflow.com/a/16361724/4379569
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    //printf("%c\n", buf);
    return buf;
 }


void clrscr() {
    printf("\e[0;0H\e[2J\e[0m");
}

void hide_cursor() {
        printf("\e[?25l");
}

void show_cursor() {
        printf("\e[?25h");
}