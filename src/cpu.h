//
// Created by henry on 5/29/24.
//

#ifndef CHIP_8_CPU_H
#define CHIP_8_CPU_H

#include "mem.h"
#include "display.h"

void set_register(uint8_t i, uint8_t val);
uint8_t* get_register_ptr(uint8_t i);

void fde(Memory mem, Display display);

#endif //CHIP_8_CPU_H
