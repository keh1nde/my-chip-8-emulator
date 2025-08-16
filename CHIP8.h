//
// Created by Kehinde Adeoso on 8/9/25.
//

#ifndef MY_CHIP_8_EMULATOR_CHIP8_H
#define MY_CHIP_8_EMULATOR_CHIP8_H

#include <vector>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>

struct CHIP8Context {
    typedef unsigned char WORD;
    typedef unsigned char BYTE;

    BYTE m_GameMemory[0xFFF]; // 0xFFF bytes of memory
    BYTE m_Registers[16]; // 16 registers, 1 byte each
    WORD m_AddressI; // The 16-bit address register I
    WORD m_ProgramCounter; // the 16-bit program counter
    std::stack<uint16_t> m_Stack; // the 16-bit stack
    BYTE m_Keypad[16];
    BYTE m_ScreenData[64][32];

    void CPUReset();
    void execute();
    WORD GetNextOpcode();


    // Helper functions
    bool isKeyPressed(const BYTE& key) const;

    // OPCodes

    /**
    * CHIP8 instruction 1NNN
    * Sets the program counter to point to the instruction located at address NNN.
    * @param opcode The OPCode that contains the address containing the instruction.
    * @post Program Counter is set to that instruction.
    */
    void OPCode1NNN(const WORD& opcode);

    /**
    * CHIP8 instruction 2NNN.
    * @param opcode The OPCode that contains the address containing the instruction.
    * @post Calls subroutine at NNN.
    */
    void OPCode2NNN(const WORD& opcode);
    /**
    * CHIP8 instruction 00E0
    * @post Clears the screen.
    */
    void OPCode00E0();

    /**
     * CHIP8 instruction 00EE
     * @post Returns to a subroutine.
     */
    void OPCode00EE();

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
    void OPCodeEX9E(const WORD& opcode);

    /**
     * CHIP8 Instruction EXA1.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Skips the next instruction if the key corresponding to the value of VX is pressed.
     */
    void OPCodeEXA1(const WORD& opcode);

    /**
     * CHIP8 Instruction FX07.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Sets VX to the value of the delay timer.
     */
    void OPCodeFX07(const WORD& opcode);

    /**
     * CHIP8 Instruction FX0A.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post A key press is awaited, and then stored in VX. All instructions are halted until the next key event.
     */
    void OPCodeFX0A(const WORD& opcode);

    /**
     * CHIP8 Instruction FX15.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Sets the delay timer to VX.
     */
    void OPCodeFX15(const WORD& opcode);

    /**
     * CHIP8 Instruction FX18.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Sets the sound timer to VX.
     */
    void OPCodeFX18(const WORD& opcode);

    /**
     * CHIP8 Instruction FX1E.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Sets the delay timer to VX.
     */
    void OPCodeFX1E(const WORD& opcode);

    /**
     * CHIP8 Instruction FX29.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Sets I to the location of the sprite for the character in VX(only consider the lowest nibble).
     * Characters 0-F (in hexadecimal) are represented by a 4x5 font
     */
    void OPCodeFX29(const WORD& opcode);

    /**
     * CHIP8 Instruction FX33.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Stores the binary-coded decimal representation of VX,
     * with the hundreds digit in memory at location in I,
     * the tens digit at location I+1, and the ones digit at location I+2.
     */
    void OPCodeFX33(const WORD& opcode);

    /**
     * CHIP8 Instruction FX55.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Stores from V0 to VX (including VX) in memory, starting at address I.
     * The offset from I is increased by 1 for each value written, but I itself is left unmodified.
     */
    void OPCodeFX55(const WORD& opcode);

    /**
     * CHIP8 Instruction FX65.
     * @param opcode An OPCode containing a number X corresponding to the requested register.
     * @post Fills from V0 to VX (including VX) with values from memory, starting at address I.
     * The offset from I is increased by 1 for each value read, but I itself is left unmodified.
     */
    void OPCodeFX65(const WORD& opcode);

};


#endif //MY_CHIP_8_EMULATOR_CHIP8_H
