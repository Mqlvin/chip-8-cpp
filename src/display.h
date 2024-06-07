//
// Created by henry on 5/31/24.
//

#ifndef CHIP_8_DISPLAY_H
#define CHIP_8_DISPLAY_H

#include <cstdint>
#include <iostream>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

struct Display {
    bool display[SCREEN_WIDTH * SCREEN_HEIGHT];
    // each consecutive 64 bools is one of 32 pixel layers
};

void set_pixel(bool display[], uint8_t x, uint8_t y, bool state);
void clear_display(bool display[]);

void test_display(const bool display[]);

#endif //CHIP_8_DISPLAY_H