//
// Created by henry on 5/31/24.
//

#include "display.h"

#include <iostream>

void set_pixel(bool display[], uint8_t x, uint8_t y, bool state) {
    display[y * SCREEN_WIDTH + x] =  state;
}

void clear_display(bool display[]) {
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        display[i] = false;
    }
}

void test_display(const bool display[]) {
    system("clear");
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        if(i % 64 == 0) {
            std::cout << '\n';
        }

        std::cout << (display[i] ? "██" : "  ");
    }
    std::cout << '\n';
}