//
// Created by Kehinde Adeoso on 8/9/25.
//
#include "CHIP8.h"


void CHIP8Context::CPUReset() {
    m_AddressI = 0 ;
    m_ProgramCounter = 0x200;

    m_DelayTimer = 0;
    m_SoundTimer = 0;

    // set all registers to 0
    memset(m_Registers,0,sizeof(m_Registers));
    memset(m_Keypad, 0, sizeof(m_Keypad));
    memset(m_ScreenData, 0, sizeof(m_ScreenData));

    // Load ROM safely (relative path recommended)
    const size_t loadOffset = 0x200;                    // Programs start at 0x200
    const size_t memSize    = 0x1000;                   // 4 KB total
    const size_t capacity   = memSize - loadOffset;     // bytes available for ROM

    // Zero out RAM before loading
    std::memset(m_GameMemory, 0, memSize);

    FILE* in = std::fopen("/Users/kehindeslaptop/Code/projects/my-chip-8-emulator/Pong.ch8", "rb"); // use relative path;
    if (!in) {
        // Could not open ROM; avoid crashing on fread(nullptr)
        // You may want to surface an error to the user/log here
        perror("Failed to open ROM");
        return;
    }

    // Read up to capacity bytes starting at 0x200
    const size_t bytesRead = std::fread(&m_GameMemory[loadOffset], 1, capacity, in);
    (void)bytesRead; // optionally log bytesRead
    std::fclose(in);
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
                break; // <— prevent fallthrough to 0x0020
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
            break; // end of case 0xF000
    }
}


// Helper functions

bool CHIP8Context::isKeyPressed(const BYTE &key) const {
    if (key > 0x0F) {
        return false; // invalid key
    }

    return m_Keypad[key] != 0;
}

void CHIP8Context::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    int scale = 10; // 64*10=640, 32*10=320
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            if (m_ScreenData[x][y]) {
                SDL_Rect rect = { x * scale, y * scale, scale, scale };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void CHIP8Context::processInput(CHIP8Context& chip8, SDL_Event& e, bool& running) {
    // Update internal keyboard state snapshot
    SDL_PumpEvents();

    // Handle window close without draining keyboard events
    if (SDL_QuitRequested()) {
        running = false;
        return;
    }

    // Map current keyboard state to CHIP-8 keypad (0..F)
    const Uint8* s = SDL_GetKeyboardState(nullptr);

    chip8.m_Keypad[0x1] = s[SDL_SCANCODE_1];
    chip8.m_Keypad[0x2] = s[SDL_SCANCODE_2];
    chip8.m_Keypad[0x3] = s[SDL_SCANCODE_3];
    chip8.m_Keypad[0xC] = s[SDL_SCANCODE_4];

    chip8.m_Keypad[0x4] = s[SDL_SCANCODE_Q];
    chip8.m_Keypad[0x5] = s[SDL_SCANCODE_W];
    chip8.m_Keypad[0x6] = s[SDL_SCANCODE_E];
    chip8.m_Keypad[0xD] = s[SDL_SCANCODE_R];

    chip8.m_Keypad[0x7] = s[SDL_SCANCODE_A];
    chip8.m_Keypad[0x8] = s[SDL_SCANCODE_S];
    chip8.m_Keypad[0x9] = s[SDL_SCANCODE_D];
    chip8.m_Keypad[0xE] = s[SDL_SCANCODE_F];

    chip8.m_Keypad[0xA] = s[SDL_SCANCODE_Z];
    chip8.m_Keypad[0x0] = s[SDL_SCANCODE_X];
    chip8.m_Keypad[0xB] = s[SDL_SCANCODE_C];
    chip8.m_Keypad[0xF] = s[SDL_SCANCODE_V];
}


// OPCodes

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
    int x = (opcode & 0x0F00) >> 8;
    int NN = (opcode & 0x00FF);

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
    const int x = (opcode & 0x0F00) >> 8;
    const int NN = (opcode & 0x00FF);

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
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

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
    int x = (opcode & 0x0F00) >> 8;
    int NN = (opcode & 0x00FF);

    m_Registers[x] = NN;
}

/**
* CHIP8 instruction 7XNN.
* @param opcode OPCode containing both X and NN.
* @post Adds NN to X.
*/
void CHIP8Context::OPCode7XNN(const WORD& opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int NN = (opcode & 0x00FF);

    m_Registers[x] += NN;
}

/**
* CHIP8 Instruction 8XY0.
* @param opcode OPCode containing both X and Y.
* Sets Vx equal to Vy.
*/
void CHIP8Context::OPCode8XY0(const WORD& opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    m_Registers[x] = m_Registers[y];
}

/**
* CHIP8 Instruction 8XY0
* @param opcode OPCode containing both X and Y
* Sets the register X equal to the bitwise OR of X and Y.
*/
void CHIP8Context::OPCode8XY1(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    m_Registers[x] = (m_Registers[x] | m_Registers[y]);
}

/**
* CHIP8 Instruction 8XY2.
* @param opcode OPCode containing both X and Y.
* Sets Vx to the value of the bitwise AND of Vx and Vy.
*/
void CHIP8Context::OPCode8XY2(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    m_Registers[x] = (m_Registers[x] & m_Registers[y]);
}

/**
* CHIP8 Instruction 8XY3.
* @param opcode OPCode containing both X and Y.
* Sets the register X equal to the XOR of X and Y.
*/
void CHIP8Context::OPCode8XY3(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    m_Registers[x] = (m_Registers[x] ^ m_Registers[y]);
}

/**
* CHIP8 Instruction 8XY4.
* @param opcode The OPCode that contains the registers X and Y.
* @post Adds Vy to Vx. Sets VF to 1 if there's an overflow.
*/
void CHIP8Context::OPCode8XY4(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    unsigned sum = static_cast<unsigned>(m_Registers[x]) + static_cast<unsigned>(m_Registers[y]);
    m_Registers[0xF] = (sum > 0xFF) ? 1 : 0;   // carry flag
    m_Registers[x]   = static_cast<BYTE>(sum & 0xFF);     // low 8 bits
}

/**
* CHIP8 Instruction 8XY5.
* @param opcode The OPCode that contains the registers X and Y.
* @post Vy is subtracted from Vx. Sets VF to 0 when there's an underflow, and 1 if not.
*/
void CHIP8Context::OPCode8XY5(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8, y = (opcode & 0x00F0) >> 4;
    m_Registers[0xF] = (m_Registers[x] >= m_Registers[y]) ? 1 : 0;
    m_Registers[x] = static_cast<BYTE>(m_Registers[x] - m_Registers[y]);
}

/**
* CHIP8 Instruction 8XY6.
* @param opcode The OPCode that contains the registers X and Y.
* @post Shifts VX to the right by 1. Stores the least significant bit of VX prior to the shift into VF.
*/
void CHIP8Context::OPCode8XY6(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;

    m_Registers[0xF] = (m_Registers[x] & 1);
    m_Registers[x] >>= 1;

}

/**
* CHIP8 Instruction 8XY7.
* @param opcode The OPCode that contains the registers X and Y.
* @post VX is set to Vx - Vy. Sets VF to 0 if an underflow occurs, and 1 if not.
*/
void CHIP8Context::OPCode8XY7(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8, y = (opcode & 0x00F0) >> 4;
    m_Registers[0xF] = (m_Registers[y] >= m_Registers[x]) ? 1 : 0;
    m_Registers[x] = static_cast<BYTE>(m_Registers[y] - m_Registers[x]);
}

/**
* CHIP8 Instruction 8XYE.
* @param opcode The OPCode that contains the registers X and Y.
* @post VX is shifted right 1. Sets VF to the least significant bit pre-shift.
*/
void CHIP8Context::OPCode8XYE(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    m_Registers[0xF] = (m_Registers[x] >> 7) & 1;
    m_Registers[x] <<= 1;
}

/**
* CHIP8 Instruction 9XY0.
* @param opcode The OPCode that contains the registers X and Y.
* @post If Vx and Vy are not equal, the next instruction is skipped.
*/
void CHIP8Context::OPCode9XY0(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;

    if (m_Registers[x] != m_Registers[y]) {
        m_ProgramCounter+=2;
    }
}

void CHIP8Context::OPCodeANNN(const WORD &opcode) {
    int address = (opcode & 0x0FFF);
    m_AddressI = address;
}

void CHIP8Context::OPCodeBNNN(const WORD &opcode) {
    m_ProgramCounter = (opcode & 0x0FFF) + m_Registers[0x0];
}

void CHIP8Context::OPCodeCXNN(const WORD &opcode) {
    std::random_device rd;   // Non-deterministic seed
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<int> dist(0, 255);

    int x = (opcode & 0x0F00) >> 8;
    int NN = opcode & 0x00FF;
    m_Registers[x] = static_cast<BYTE>(dist(gen) & NN);

}


void CHIP8Context::OPCodeDXYN(const WORD& opcode) {
    int x = (opcode & 0x0F00) >> 8;
    int y = (opcode & 0x00F0) >> 4;
    int height = opcode & 0x000F;   // N rows

    BYTE Vx = m_Registers[x];
    BYTE Vy = m_Registers[y];

    m_Registers[0xF] = 0; // reset collision flag

    for (int row = 0; row < height; ++row) {
        BYTE spriteByte = m_GameMemory[m_AddressI + row];

        // Each sprite row is 8 pixels wide, MSB on the left
        for (int bit = 0; bit < 8; ++bit) {
            if (spriteByte & (0x80 >> bit)) { // pixel should be drawn
                int px = (Vx + bit) & 63;  // wrap 0..63
                int py = (Vy + row) & 31;  // wrap 0..31

                BYTE &screenPixel = m_ScreenData[px][py];
                if (screenPixel == 1) {
                    m_Registers[0xF] = 1; // collision
                }
                screenPixel ^= 1; // XOR draw
            }
        }
    }
}



void CHIP8Context::OPCodeEX9E(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8; // extract X
    BYTE key = m_Registers[x] & 0x000F; // lowest nibble

    if (isKeyPressed(key)) {
        m_ProgramCounter += 2; // skip next instruction
    }
}

void CHIP8Context::OPCodeEXA1(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    BYTE key = m_Registers[x] & 0x000F;

    if (!isKeyPressed(key)) {
        m_ProgramCounter +=2;
    }
}

void CHIP8Context::OPCodeFX0A(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;

    bool keyPressed = false;
    for (int i = 0; i < 16; ++i) {
        if (m_Keypad[i]) {
            m_Registers[x] = i;
            keyPressed = true;
            break;
        }
    }

    // If no key is pressed, decrement PC to re-execute this instruction
    if (!keyPressed) {
        m_ProgramCounter -= 2;
    }
}

void CHIP8Context::OPCodeFX1E(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    m_AddressI += m_Registers[x];
}

void CHIP8Context::OPCodeFX07(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    m_Registers[x] = m_DelayTimer;
}

void CHIP8Context::OPCodeFX15(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    m_DelayTimer = m_Registers[x];
}

void CHIP8Context::OPCodeFX18(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    m_SoundTimer = m_Registers[x];
}

void CHIP8Context::OPCodeFX29(const WORD &opcode) {
    // FX29: Set I = location of sprite for digit Vx.
    int x = (opcode & 0x0F00) >> 8;
    BYTE digit = m_Registers[x] & 0x0F; // 0..F

    // Standard CHIP-8 fontset base (each glyph 5 bytes)
    const WORD FONT_BASE = 0x050;
    const WORD GLYPH_SIZE = 5;

    m_AddressI = static_cast<WORD>(FONT_BASE + static_cast<WORD>(digit) * GLYPH_SIZE);
}

void CHIP8Context::OPCodeFX33(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;
    BYTE value = m_Registers[x];

    // Store BCD representation of Vx in memory
    m_GameMemory[m_AddressI] = value / 100;        // hundreds
    m_GameMemory[m_AddressI + 1] = (value / 10) % 10;  // tens
    m_GameMemory[m_AddressI + 2] = value % 10;         // ones
}

void CHIP8Context::OPCodeFX55(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; i++) {
        m_GameMemory[m_AddressI + i] = m_Registers[i];
    }
}

void CHIP8Context::OPCodeFX65(const WORD &opcode) {
    int x = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= x; i++) {
        m_Registers[i] = m_GameMemory[m_AddressI + i];
    }
}
