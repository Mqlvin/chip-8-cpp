//
// Created by henry on 5/29/24.
//

#include "util.h"

uint8_t get_first_nibble(uint8_t val) {
    return val >> 4;
}

uint8_t get_last_nibble(uint8_t val) {
    return (val & 0xFF00) >> 4;
}
