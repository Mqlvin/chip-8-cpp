//
// Created by henry on 5/29/24.
//

#ifndef CHIP_8_MEM_H
#define CHIP_8_MEM_H

#include <cstdint>
#include <stdexcept>

#define MAX_BYTES 4096

struct Memory {
    uint8_t memory[MAX_BYTES];
};

void set(uint8_t mem[], uint16_t i, uint8_t val);
uint8_t get(uint8_t mem[], uint16_t i);

#endif //CHIP_8_MEM_H
