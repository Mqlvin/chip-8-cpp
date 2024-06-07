//
// Created by henry on 5/29/24.
//

#include "mem.h"

#include <cstdint>
#include <stdexcept>

void set(uint8_t mem[], uint16_t i, uint8_t val) {
    if(i > MAX_BYTES - 1) {
        throw std::out_of_range("SETTING memory index (den:" + std::to_string(i) + + ") out of range (den:" + std::to_string(MAX_BYTES) + ")");
    }
    mem[i] = val;
}

uint8_t get(uint8_t mem[], uint16_t i) {
    if(i > MAX_BYTES - 1) {
        throw std::out_of_range("INDEXING memory index (den:" + std::to_string(i) + + ") out of range (den:" + std::to_string(MAX_BYTES) + ")");
    }
    return mem[i];
}