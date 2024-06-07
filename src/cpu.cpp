//
// Created by henry on 5/29/24.
//

#include "cpu.h"

#include "mem.h"
#include "display.h"

#include <iostream>
#include <cstdint>
#include <stack>
#include <stdexcept>

uint16_t reg_pc {};
uint16_t reg_i {};
std::stack<uint16_t> stack {}; // non fixed size, hope there are no infinite recursions...
uint8_t reg_gp[0xF]; // general purpose registers


void set_register(uint8_t i, uint8_t val) {
    reg_gp[i] = val;
}

uint8_t* get_register_ptr(uint8_t i) {
    return &reg_gp[i];
}

void pop_stack() {
    stack.pop();
}

void push_stack(uint16_t val) {
    stack.push(val);
}


#define mask_xh(o) ((o & 0xF000) >> 12) // higher nibble of high byte
#define mask_xl(o) ((o & 0x0F00) >> 8) // lower nibble of high byte
#define mask_yh(o) ((o & 0x00F0) >> 4) // higher nibble of low byte
#define mask_yl(o) (o & 0x000F) // lower nibble of low byte
#define mask_trailing(o) (o & 0x0FFF) // all nibbles except high nibble of high byte
#define mask_lb(o) (o & 0x00FF) // low byte

void fde(Memory mem, Display display) {
    // fetch

    uint16_t inst = ((mem.memory[reg_pc]) << 8) + mem.memory[reg_pc + 1];
//    std::cout << "Test " << static_cast<int>(mask_yh(inst)) << "\n";
// test parsing ^^^^

    uint8_t opcode = mem.memory[reg_pc] >> 4;
    uint8_t nib_2 = (mem.memory[reg_pc] & 0x0F);
    uint8_t nib_3 = mem.memory[reg_pc + 1] >> 4;
    uint8_t nib_4 = (mem.memory[reg_pc + 1] & 0x0F);
    std::cout << "Instruction: <" << static_cast<int>(opcode) << "-" << static_cast<int>(nib_2) << "-" << static_cast<int>(nib_3) << "-" << static_cast<int>(nib_4) << ">\n";


    reg_pc += 2; // increment pc by 2

    // not my favourite switch-case.
    // instructions can be found here https://en.wikipedia.org/wiki/CHIP-8#Opcode_table
    switch(opcode) { // first nibble of instruction
        case 0x0: {
            switch (nib_3) {
                case 0xE: {

                    switch(nib_4) {
                        case 0x0: {
                            // clear the display
                            clear_display(display.display);

                            break;
                        }

                        case 0xE: {
                            // return from subroutine
                            reg_pc = stack.top();
                            stack.pop();

                            break;
                        }

                        default: {
                            throw std::invalid_argument("Invalid argument under OPCODE 0x0");
                        }
                    }

                    break;
                }

                default: {
                    throw std::invalid_argument("Invalid argument under OPCODE 0x0");
                }
            }

        }


        case 0x1: {
            // jump to nnn (set pc)
            reg_pc = mask_trailing(inst);

            break;
        }


        case 0x2: {
            // call subroutine at nnn (push pc to stack, set pc)
            stack.push(reg_pc);
            reg_pc = mask_trailing(inst);

            break;
        }


        case 0x3: {
            // skip next instruction if vx == nn
            if(*get_register_ptr(mask_xl(inst)) == mask_lb(inst)) reg_pc += 2;

            break;
        }


        case 0x4: {
            // skip next instruction if vx != nn
            if(*get_register_ptr(mask_xl(inst)) != mask_lb(inst)) reg_pc += 2;

            break;
        }


        case 0x5: {
            switch(nib_4) {
                case 0x0: {
                    // skip next instruction if vx == vy
                    if(*get_register_ptr(mask_xl(inst)) == *get_register_ptr(mask_yh(inst))) reg_pc += 2;

                    break;
                }

                default: {
                    throw std::invalid_argument("Invalid argument under OPCODE 0x5");
                }
            }

            break;
        }


        case 0x6: {
            // set vx to nn
            set_register(mask_xl(inst), mask_lb(inst));

            break;
        }


        case 0x7: {
            // add nn to vx (don't change carry flag)
            set_register(mask_xl(inst), *get_register_ptr(mask_xl(inst)) + mask_lb(inst));

            break;
        }


        case 0x8: {
            switch(nib_4) {
                case 0x0: {
                    // set vx to vy
                    set_register(mask_xl(inst), *get_register_ptr(mask_yh(inst)));

                    break;
                }

                case 0x1: {
                    // set vx to vx OR vy (bitwise)
                    set_register(mask_xl(inst), *get_register_ptr(mask_xl(inst)) | *get_register_ptr(mask_yh(inst)));

                    break;
                }

                case 0x2: {
                    // set vx to vx AND vy (bitwise)
                    set_register(mask_xl(inst), *get_register_ptr(mask_xl(inst)) & *get_register_ptr(mask_yh(inst)));

                    break;
                }

                case 0x3: {
                    // set vx to vx XOR vy (bitwise)
                    set_register(mask_xl(inst), *get_register_ptr(mask_xl(inst)) ^ *get_register_ptr(mask_yh(inst)));

                    break;
                }

                case 0x4: {
                    // add vy to vx. vf is set to 1 if overflow, 0 if no overflow
                    uint8_t previous_x_ptr = *get_register_ptr(mask_xl(inst));
                    set_register(mask_yh(inst), previous_x_ptr + *get_register_ptr(mask_yh(inst)));
                    *get_register_ptr(0xF) = static_cast<uint8_t>(previous_x_ptr < *get_register_ptr(mask_yh(inst)));

                    break;
                }

                case 0x5: {
                    // vy subtracted from vx. vf set to 0 if underflow, 1 if no underflow
                    uint8_t previous_x_ptr = *get_register_ptr(mask_xl(inst));
                    set_register(mask_yh(inst), previous_x_ptr - *get_register_ptr(mask_yh(inst)));
                    *get_register_ptr(0xF) = static_cast<uint8_t>(previous_x_ptr > *get_register_ptr(mask_yh(inst)));

                    break;
                }

                case 0x6: {
                    // todo shift vx to the right by 1, wrap least significant bit around to most

                    break;
                }

                case 0x7: {
                    // todo set vx to vy - vx. set to 0 if underflow, 1 if no underflow

                    break;
                }

                case 0xE: {
                    // todo shift vx to the left by 1, wrap least significant bit around to most

                    break;
                }

                default: {
                    throw std::invalid_argument("Invalid argument under OPCODE 0x8");
                }
            }
        }


        case 0x9: {
            switch(nib_4) {
                case 0x0: {
                    // todo skip next instruction if vx != vy

                    break;
                }

                default: {
                    throw std::invalid_argument("Invalid argument under OPCODE 0x9");
                }
            }
        }


        case 0xA: {
            // todo set register i to nnn

            break;
        }


        case 0xB: {
            // todo move pc to address nnn + value of v0

            break;
        }


        case 0xC: {
            // todo set vx to: generate random number between 0x00 and 0xFF, perform bitwise AND with nn

            break;
        }


        case 0xD: {
            // todo draw sprite at coordinate vx, vy with height of 8px and width of npx

            break;
        }


        case 0xE: {
            switch((nib_3 << 4) + nib_4) {
                case 0x9E: {
                    // todo skip next instruction if key in vx in pressed

                    break;
                }

                case 0xA1: {
                    // todo skip next instruction if key stored in vx NOT pressed

                    break;
                }

                default: {
                    throw std::invalid_argument("Invalid argument under OPCODE 0xE");
                }
            }
        }


        case 0xF: {
            switch((nib_3 << 4) + nib_4) {
                case 0x07: {
                    // todo set vx to value of delay timer

                    break;
                }

                case 0x0A: {
                    // todo block all instruction until key pressed, store key id in vx

                    break;
                }

                case 0x15: {
                    // todo set delay timer to vx

                    break;
                }

                case 0x18: {
                    // todo set sound timer to vx

                    break;
                }

                case 0x1E: {
                    // todo add vx to register i. vf not affected.

                    break;
                }

                case 0x29: {
                    // todo set register i to location of spriter for character in vx.

                    break;
                }

                case 0x33: {
                    // todo store binary coded decimal value of vx, whereabouts found on wikipedia linked above

                    break;
                }

                case 0x55: {
                    // todo store v0 to vx in memory, starting at address in register i. don't modify i.

                    break;
                }

                case 0x65: {
                    // todo fill v0 to vx with values from memory, starting at address in register i. don't modify i.

                    break;
                }

                default: {
                    throw std::invalid_argument("Invalid argument under OPCODE 0xF");
                }
            }
        }


        default: {
            throw std::invalid_argument("Invalid OPCODE");
        }
    }
}