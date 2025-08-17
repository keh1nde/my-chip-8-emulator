# my-chip-8-emulator


This is a CHIP-8 emulator written entirely in C++ with SDL2 for graphics rendering.  I completed this project over the course of about a week to serve as both a portfolio piece and a hands on experience of computer architecture, CPU design principles, and modern C++ practices, an experience I was not able to get in class.

## Overview

The CHIP-8 is a simple interpreted programming language from the 1970s, commonly used today as a learning platform for emulation. This emulator faithfully reproduces the original instruction set, allowing users to play games like Tetris, Pong, and Space Invaders.

## Key highlights:
- Full CPU implementation of all CHIP-8 opcodes.
- (Mostly) working graphics via SDL2, with some flickering depending on the ROM.
- Engineered for portability – tested on macOS, but should run on Windows and Linux with minimal modification.

- Emphasis on low-level programming techniques, including:

  - Bit manipulation (masking, shifting, decoding opcodes).
  - Exception handling and memory management.
  - Understanding of CPU cycles, program counters, registers, and memory layout.

## Project Outcomes

This project deepened my understanding of:
- Computer Architecture: how CPUs fetch, decode, and execute instructions.
- Memory Management: stack operations, program counters, and address space organization.
- C++ Best Practices: modular code design, exception handling, and low-level data manipulation.
- Graphics Programming: rendering frames, managing buffers, and timing loops via SDL2.

How to Build
1.	Make sure you have a C++17 (or later) compiler installed.
2.	Install SDL2 on your system.
•	On macOS with Homebrew:

`brew install sdl2`


- On Linux (Ubuntu/Debian):

`sudo apt-get install libsdl2-dev`


- On Windows, download the development libraries and configure them with your compiler/IDE.

3. Clone the repository and build:

```
git clone https://github.com/yourusername/chip8-emulator.git
cd chip8-emulator
g++ -std=c++17 main.cpp -lSDL2 -o chip8
```


## How to Use

1. This emulator requires a CHIP-8 ROM. Pong is already included for demonstration purposes, but if you'd like, you can get other ones [here](https://github.com/dmatlack/chip8/tree/master/roms).
2. Set the absolute filepath of the ROM on line 27 of CHIP8.cpp.
3. Build and Run.


## Future Improvements
- Reduce or eliminate flickering across ROMs.
- Add sound support (beep timer).
- Improved packaging for easier setup.

## Final Notes

I undertook this project to learn more about low-level programming with the aim of eventually working with it someday. I hope you enjoy using this emulator as much as I enjoyed making it.
