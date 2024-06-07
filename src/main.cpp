#include <iostream>

#include "mem.h"
#include "cpu.h"
#include "display.h"

int main() {
    Memory mem{};
    Display display{};

    set(mem.memory, 0x00, 0x56);
    set(mem.memory, 0x01, 0xA0);
    fde(mem, display);

    return 0;
}
