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