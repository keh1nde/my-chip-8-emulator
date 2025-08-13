//
// Created by Kehinde Adeoso on 8/9/25.
//
#include "CHIP8.h"


void CHIP8Context::CPUReset() {
    m_AddressI = 0 ;
    m_ProgramCounter = 0x200;
    memset(m_Registers,0,sizeof(m_Registers)) ; // set registers to 0

    // load in the game
    FILE *in = fopen("c:/INVADERS", "rb");
    fread( &m_GameMemory[0x200], 0xfff, 1, in);
    fclose(in);
}

CHIP8Context::WORD CHIP8Context::GetNextOpcode()
{
    WORD res = 0 ;
    res = m_GameMemory[m_ProgramCounter] ; // in example res is 0xAB
    res <<= 8 ; // shift 8 bits left. In our example res is 0xAB00
    res |= m_GameMemory[m_ProgramCounter+1] ; // In example res is 0xABCD
    m_ProgramCounter+=2 ;
    return res ;
}

void CHIP8Context::execute() {
    const WORD opcode = GetNextOpcode();

    switch (opcode & 0xF000) { // The first character
        case 0x1000:
            OPCode1NNN(opcode);
            break;

        case 0x0000: // Requires further analysis
            switch (opcode & 0x0FFF) {
                case 0x00E0:
                    OPCode00E0();
                    break;
            case 0x00EE:
                    OPCode00EE();
                    break;
            }

        case 0x2000:
            // Full instruction: 0x2NNN. Call subroutine at NNN.

        case 0x3000:
            OPCode3XNN(opcode);
            break;

        case 0x4000:
            OPCode4XNN(opcode);
            break;

        case 0x5000:
            OPCode5XY0(opcode);
            break;

        case 0x6000:
            OPCode6XNN(opcode);
            break;

        case 0x7000:
            OPCode7XNN(opcode);
            break;

        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    OPCode8XY0(opcode);
                    break;
                case 0x0001:
                    OPCode8XY1(opcode);
                    break;
                case 0x0002:
                    OPCode8XY2(opcode);
                    break;
                case 0x0003:
                    OPCode8XY3(opcode);
                    break;

            }


        default: return; // No implementation.
    }
}

/**
 * CHIP8 instruction 1NNN: Call
 * Sets the program counter to point to the instruction located at address NNN.
 * @param opcode
 */
void CHIP8Context::OPCode1NNN(const WORD& opcode) {
    m_ProgramCounter = opcode & 0x0FFF;
}

/**
 CHIP8 instruction 00E0: Clears the screen.
 */
void CHIP8Context::OPCode00E0() { // Read the chapter on Graphics before implementing.
    return;
}

/**
* CHIP8 instruction 00EE: Returns to a subroutine.
*/
void CHIP8Context::OPCode00EE() const {
    return;
}

/**
* CHIP8 instruction 3XNN
* @param opcode OPCode that contains X and NN.
* @post Check if X and NN are equal, and skips the next instruction if true.
*/
void CHIP8Context::OPCode3XNN(const WORD& opcode) {
    const WORD Vx = (opcode & 0x0F00);
    const WORD NN = (opcode & 0x00FF);

    if (Vx == NN) {
        m_ProgramCounter += 2; // Skip the next instruction.
    }
}

/**
* CHIP8 instruction 4XNN.
* @param opcode OPCode that contains X and NN.
* @post Check if X and NN are not equal, and skips the next instruction if true.
*/
void CHIP8Context::OPCode4XNN(const WORD& opcode) {
    const WORD Vx = (opcode & 0x0F00);
    const WORD NN = (opcode & 0x00FF);

    if (Vx != NN) {
        m_ProgramCounter += 2; // Skip the next instruction.
    }
}

/**
* CHIP8 instruction 5XY0
* @param opcode OPCode containing both X and Y
* @post Check if X and Y are the same, and skips the next instruction if so.
*/
void CHIP8Context::OPCode5XY0(const WORD &opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE y = (opcode & 0x00F0);

    if (m_Registers[x] == m_Registers[y]) {
        m_ProgramCounter += 2; // Skip the next instruction.
    }
}

/**
* CHIP8 instruction 6XNN.
* @param opcode OPCode containing both X and NN.
* @post Sets X equal to NN.
*/
void CHIP8Context::OPCode6XNN(const WORD &opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE NN = (opcode & 0x00FF);

    m_Registers[x] = NN;
}

/**
* CHIP8 instruction 7XNN.
* @param opcode OPCode containing both X and NN.
* @post Adds NN to X.
*/
void CHIP8Context::OPCode7XNN(const WORD& opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE NN = (opcode & 0x00FF);

    m_Registers[x] += NN;
}

/**
* CHIP8 Instruction 8XY0.
* @param opcode OPCode containing both X and Y.
* Sets the register X equal to Y.
*/
void CHIP8Context::OPCode8XY0(const WORD& opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE NN = (opcode & 0x00F0);

    m_Registers[x] = NN;
}

/**
* CHIP8 Instruction 8XY0
* @param opcode OPCode containing both X and Y
* Sets the register X equal to the bitwise OR of X and Y.
*/
void CHIP8Context::OPCode8XY1(const WORD &opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE y = (opcode & 0x00F0);

    m_Registers[x] = x|y;
}

/**
* CHIP8 Instruction 8XY0
* @param opcode OPCode containing both X and Y
* Sets the register X equal to Y.
*/
void CHIP8Context::OPCode8XY2(const WORD &opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE y = (opcode & 0x00F0);

    m_Registers[x] = x&y;
}

/**
* CHIP8 Instruction 8XY3
* @param opcode OPCode containing both X and Y
* Sets the register X equal to the XOR of X and Y.
*/
void CHIP8Context::OPCode8XY3(const WORD &opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE y = (opcode & 0x00F0);

    m_Registers[x] = x^y;
}
