#pragma once
#include <stdint.h>
uint8_t sub_or_zero(uint8_t a, uint8_t b);
uint8_t cap_max(uint8_t x, uint8_t max);
unsigned invalid_id(uint8_t id, uint8_t max);

char getch(void);

void hide_cursor();

void show_cursor();

void clrscr();