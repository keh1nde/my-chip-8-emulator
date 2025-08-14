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

        case 0x0000: // Requires further analysis. This is done for all instructions with common digits.
            switch (opcode & 0x0FFF) {
                case 0x00E0:
                    OPCode00E0();
                    break;
                case 0x00EE:
                    OPCode00EE();
                    break;
                default: // Fatal Error. Incorrect instruction.
                    break;
            }
            break;

        case 0x2000:
            OPCode2NNN(opcode);
            break;

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
                case 0x0004:
                    OPCode8XY4(opcode);
                    break;
                case 0x0005:
                    OPCode8XY5(opcode);
                    break;
                case 0x0006:
                    OPCode8XY6(opcode);
                    break;
                case 0x0007:
                    OPCode8XY7(opcode);
                    break;
                case 0x000E:
                    OPCode8XYE(opcode);
                    break;
                default: // An error has occurred.
                    break;
            }
            break;
        case 0x9000:
            OPCode9XY0(opcode);
             break;
        case 0xA000:
            OPCodeANNN(opcode);
            break;
        case 0xB000:
            OPCodeBNNN(opcode);
            break;
        case 0xC000:
            OPCodeCXNN(opcode);
            break;
        case 0xD000:
            OPCodeDXYN(opcode);
            break;
        case 0xE000:
            switch (opcode & 0x000F){
                case 0x000E:
                    OPCodeEX9E(opcode);
                    break;
                case 0x0001:
                    OPCodeEXA1(opcode);
                    break;
                default: // An error has occurred.
                    break;
            }
            break;

        case 0xF000:
            switch (opcode & 0x00F0) {
            case 0x0000:
                switch (opcode & 0x000F) {
                    case 0x0007:
                        OPCodeFX07(opcode);
                        break;
                    case 0x000A:
                        OPCodeFX0A(opcode);
                        break;
                default: // An error has occurred.
                        break;
                }
                break;
            case 0x0010:
                switch (opcode & 0x000F) {
                    case 0x0005:
                        OPCodeFX15(opcode);
                        break;
                    case 0x0008:
                        OPCodeFX18(opcode);
                        break;
                    case 0x000E:
                        OPCodeFX1E(opcode);
                        break;
                default: // An error has occurred.
                        break;
                }
            case 0x0020:
                    OPCodeFX29(opcode);
                    break;
            case 0x0030:
                    OPCodeFX33(opcode);
                    break;
            case 0x0050:
                    OPCodeFX55(opcode);
                    break;
            case 0x0060:
                    OPCodeFX65(opcode);
                    break;
            }
        default: // An error has occurred.
            break;
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
* CHIP8 instruction 00EE
* @post Returns to a subroutine.
*/
void CHIP8Context::OPCode00EE() {
    if (!m_Stack.empty()) {
        m_ProgramCounter = m_Stack.top();
        m_Stack.pop();
    } else {
        // Handle stack underflow error
        m_ProgramCounter = 0x200; // reset to start or handle error appropriately.
    }
}

/**
* CHIP8 instruction 2NNN.
* @param opcode The OPCode that contains the address containing the instruction.
* @post Calls subroutine at NNN.
*/
void CHIP8Context::OPCode2NNN(const WORD& opcode) {
    // Push current PC onto the stack
    m_Stack.push(m_ProgramCounter);

    // Extract NNN (lower 12 bits)
    const WORD address = opcode & 0x0FFF;

    // Jump to NNN
    m_ProgramCounter = address;
}
/**
 CHIP8 instruction 00E0: Clears the screen.
 */
void CHIP8Context::OPCode00E0() {
    std::memset(m_ScreenData, 0, sizeof(m_ScreenData));
}


/**
* CHIP8 instruction 3XNN
* @param opcode OPCode that contains X and NN.
* @post Check if X and NN are equal, and skips the next instruction if true.
*/
void CHIP8Context::OPCode3XNN(const WORD& opcode) {
    const WORD x = (opcode & 0x0F00);
    const WORD NN = (opcode & 0x00FF);

    if (m_Registers[x] == NN) {
        m_ProgramCounter += 2; // Skip the next instruction.
    }
}

/**
* CHIP8 instruction 4XNN.
* @param opcode OPCode that contains X and NN.
* @post Check if X and NN are not equal, and skips the next instruction if true.
*/
void CHIP8Context::OPCode4XNN(const WORD& opcode) {
    const int x = (opcode & 0x0F00);
    const WORD NN = (opcode & 0x00FF);

    if (m_Registers[x] != NN) {
        m_ProgramCounter += 2; // Skip the next instruction.
    }
}

/**
* CHIP8 instruction 5XY0.
* @param opcode OPCode containing both X and Y.
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

/**
* CHIP8 Instruction 8XY4.
* @param opcode The OPCode that contains the registers X and Y.
* @post Adds Vy to Vx. Sets VF to 1 if there's an overflow.
*/
void CHIP8Context::OPCode8XY4(const WORD &opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE y = (opcode & 0x00F0);

    if (m_Registers[y] > std::numeric_limits<int>::max() - m_Registers[y]) {
        m_Registers[0xF] = 1;
        return;
    }

    m_Registers[x] += m_Registers[y];
}

/**
* CHIP8 Instruction 8XY5.
* @param opcode The OPCode that contains the registers X and Y.
* @post Vy is subtracted from Vx. Sets VF to 0 when there's an underflow, and 1 if not.
*/
void CHIP8Context::OPCode8XY5(const WORD &opcode) {
    const BYTE x = (opcode & 0x0F00);
    const BYTE y = (opcode & 0x00F0);

    // If Vx > 0, Vx - Vy is smaller. Check to see if Vx is already close to 0.
    if (m_Registers[y] < 0 && m_Registers[x] < std::numeric_limits<int>::min() + m_Registers[y]) {
        m_Registers[0xF] = 0;
        return;
    }
    // Likewise, if Vy < 0, Vx - |Vy| is smaller. Check to see if Vx is already close to 0.
    if (m_Registers[y] < 0 && m_Registers[x] < std::numeric_limits<int>::min() - m_Registers[y]) {
        m_Registers[0xF] = 0;
        return;
    }

    m_Registers[x] -= m_Registers[y];
    m_Registers[0xF] = 1;
}

/**
* CHIP8 Instruction 8XY6.
* @param opcode The OPCode that contains the registers X and Y.
* @post Shifts VX to the right by 1. Stores the least significant bit of VX prior to the shift into VF.
*/
void CHIP8Context::OPCode8XY6(const WORD &opcode) {
    int x = (opcode & 0x0F00);

    m_Registers[0xF] = (m_Registers[x] & 1);

    m_Registers[x] = m_Registers[x] >> 8;

}

/**
* CHIP8 Instruction 8XY7.
* @param opcode The OPCode that contains the registers X and Y.
* @post VX is set to Vx - Vy. Sets VF to 0 if an underflow occurs, and 1 if not.
*/
void CHIP8Context::OPCode8XY7(const WORD &opcode) {
    int x = (opcode & 0x0F00);
    x = x >> 8;
    int y = (opcode & 0x00F0);
    y = y >> 4;

    if (m_Registers[y] < 0 && m_Registers[x] < std::numeric_limits<int>::min() + m_Registers[y]) {
        m_Registers[0xF] = 0;
        return;
    }
    // Likewise, if Vy < 0, Vx - |Vy| is smaller. Check to see if Vx is already close to 0.
    if (m_Registers[y] < 0 && m_Registers[x] < std::numeric_limits<int>::min() - m_Registers[y]) {
        m_Registers[0xF] = 0;
        return;
    }

    m_Registers[x] = m_Registers[x] - m_Registers[y];
    m_Registers[0xF] = 1;
}

/**
* CHIP8 Instruction 8XYE.
* @param opcode The OPCode that contains the registers X and Y.
* @post VX is shifted right 1. Sets VF to the least significant bit pre-shift.
*/
void CHIP8Context::OPCode8XYE(const WORD &opcode) {
    int x = (opcode & 0x0F00);
    m_Registers[0xF] = (opcode & 1);
    x = x >> 8;

    m_Registers[x] = m_Registers[x] >> 8;
}

/**
* CHIP8 Instruction 9XY0.
* @param opcode The OPCode that contains the registers X and Y.
* @post If Vx and Vy are not equal, the next instruction is skipped.
*/
void CHIP8Context::OPCode9XY0(const WORD &opcode) {
    int x = (opcode & 0x0F00);
    x = x >> 8;
    int y = (opcode & 0x00F0);
    y = y >> 4;

    if (m_Registers[x] != m_Registers[y]) {
        m_ProgramCounter+=2;
    }
}


