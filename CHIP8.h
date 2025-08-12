//
// Created by Kehinde Adeoso on 8/9/25.
//

#ifndef MY_CHIP_8_EMULATOR_CHIP8_H
#define MY_CHIP_8_EMULATOR_CHIP8_H

#include <vector>
#include <cstdint>

struct CHIP8Context {
    typedef unsigned char WORD;
    typedef unsigned char BYTE;

    BYTE m_GameMemory[0xFFF]; // 0xFFF bytes of memory
    BYTE m_Registers[16]; // 16 registers, 1 byte each
    WORD m_AddressI; // The 16-bit address register I
    WORD m_ProgramCounter; // the 16-bit program counter
    std::vector<uint16_t> m_Stack; // the 16-bit stack

    BYTE m_ScreenData[64][32];

    void CPUReset();
    WORD GetNextOpcode();
};


#endif //MY_CHIP_8_EMULATOR_CHIP8_H