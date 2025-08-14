//
// Created by Kehinde Adeoso on 8/9/25.
//

#ifndef MY_CHIP_8_EMULATOR_CHIP8_H
#define MY_CHIP_8_EMULATOR_CHIP8_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <limits>

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
     * @post Sets the register X equal to NN.
     */
    void OPCode6XNN(const WORD& opcode);

    /**
     * CHIP8 instruction 7XNN.
     * @param opcode OPCode containing both X and NN.
     * @post Adds NN to the number located in register X.
     */
    void OPCode7XNN(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY0.
     * @param opcode OPCode containing both X and Y.
     * Sets the register X equal to Y.
     */
    void OPCode8XY0(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY1.
     * @param opcode OPCode containing both X and Y.
     * Sets the register X equal to the bitwise OR of X and Y.
     */
    void OPCode8XY1(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY2.
     * @param opcode OPCode containing both X and Y.
     * Sets the register X equal to the bitwise AND of X and Y.
     */
    void OPCode8XY2(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY3.
     * @param opcode OPCode containing both X and Y.
     * Sets the register X equal to the XOR of X and Y.
     */
    void OPCode8XY3(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY4.
     * @param opcode The OPCode that contains the registers X and Y.
     * @post Adds Vy to Vx. Sets VF to 1 if there's an overflow.
     */
    void OPCode8XY4(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY5.
     * @param opcode The OPCode that contains the registers X and Y.
     * @post Vy is subtracted from Vx. Sets VF to 0 when there's an underflow, and 1 if not.
     */
    void OPCode8XY5(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY6.
     * @param opcode The OPCode that contains the registers X and Y.
     * @post Shifts VX to the right by 1. Stores the least significant bit of VX prior to the shift into VF.
     */
    void OPCode8XY6(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XY7.
     * @param opcode The OPCode that contains the registers X and Y.
     * @post VX is set to Vx - Vy. Sets VF to 0 if an underflow occurs, and 1 if not.
     */
    void OPCode8XY7(const WORD& opcode);

    /**
     * CHIP8 Instruction 8XYE.
     * @param opcode The OPCode that contains the registers X and Y.
     * @post VX is shifted right 1. Sets VF to the least significant bit pre-shift.
     */
    void OPCode8XYE(const WORD& opcode);

    /**
     * CHIP8 Instruction 9XY0.
     * @param opcode The OPCode that contains the registers X and Y.
     * @post If Vx and Vy are not equal, the next instruction is skipped.
     */
    void OPCode9XY0(const WORD& opcode);

    /**
     * CHIP8 Instruction ANNN.
     * @param opcode The OPCode that contains a memory address.
     * @post The address register I is set to the memory address in the opcode.
     */
    void OPCodeANNN(const WORD& opcode);

    /**
     * CHIP8 Instruction BNNN.
     * @param opcode The OPCode that contains a memory address.
     * @post Jumps to the memory address in the opcode plus the number located in register 0.
     */
    void OPCodeBNNN(const WORD& opcode);

    /**
     * CHIP8 Instruction CXNN.
     * @param opcode The OPCode that contains the register numbered X.
     * @post Sets VX equal to the bitwise AND of VX and a random number NN (between 0 and 255).
     */
    void OPCodeCXNN(const WORD& opcode);

    /**
     * CHIP8 Instruction DXYN.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
     * VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
     * and to 0 if that does not happen.
     */
    void OPCodeDXYN(const WORD& opcode);

    /**
     * CHIP8 Instruction EX9E.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
     * VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
     * and to 0 if that does not happen.
     */
};


#endif //MY_CHIP_8_EMULATOR_CHIP8_H
