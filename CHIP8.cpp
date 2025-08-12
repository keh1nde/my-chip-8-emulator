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

