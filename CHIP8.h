//
// Created by Kehinde Adeoso on 8/9/25.
//

#ifndef MY_CHIP_8_EMULATOR_CHIP8_H
#define MY_CHIP_8_EMULATOR_CHIP8_H

#include <vector>
#include <cstdint>
#include <iostream>

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
    void execute();
    WORD GetNextOpcode();

    // OPCodes

    /**
    * CHIP8 instruction 1NNN: Call
    * Sets the program counter to point to the instruction located at address NNN.
    * @param opcode The OPCode that contains the address containing the instruction.
    * @post Program Counter is set to that instruction.
    */
    void OPCode1NNN(const WORD& opcode);

    /**
    CHIP8 instruction 00E0: Clears the screen.
    */
    void OPCode00E0();

    /**
     * CHIP8 instruction 00EE: Returns to a subroutine.
     */
    void OPCode00EE() const;

    /**
     * CHIP8 instruction 3XNN
     * @param opcode OPCode that contains X and NN.
     * @post Check if X and NN are equal, and skips the next instruction if true.
     */
    void OPCode3XNN(const WORD& opcode);

    /**
     * CHIP8 instruction 4XNN.
     * @param opcode OPCode that contains X and NN.
     * @post Check if X and NN are not equal, and skips the next instruction if true.
     */
    void OPCode4XNN(const WORD& opcode);

    /**
     * CHIP8 instruction 5XY0
     * @param opcode OPCode containing both X and Y
     * @post Check if X and Y are the same, and skips the next instruction if so.
     */
    void OPCode5XY0(const WORD& opcode);

    /**
     * CHIP8 instruction 6XNN.
     * @param opcode OPCode containing both X and NN.
     * @post Sets X equal to NN.
     */
    void OPCode6XNN(const WORD& opcode);
};


#endif //MY_CHIP_8_EMULATOR_CHIP8_H