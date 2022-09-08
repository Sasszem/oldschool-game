#include "util.h"

uint8_t sub_or_zero(uint8_t a, uint8_t b) {
        return a>b ? a-b : 0;
}

uint8_t cap_max(uint8_t x, uint8_t max) {
        return x > max ? max : x;
}

unsigned invalid_id(uint8_t id, uint8_t max) {
        return !id || id > max;
}