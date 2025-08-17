cmake_minimum_required(VERSION 4.0)
project(my_chip_8_emulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

find_package(SDL2 REQUIRED)

add_executable(my-chip-8-emulator
        CHIP8.cpp
        CHIP8.h
        main.cpp)

# Link SDL2
target_link_libraries(my-chip-8-emulator ${SDL2_LIBRARIES})
target_include_directories(my-chip-8-emulator PRIVATE ${SDL2_INCLUDE_DIRS})
target_compile_options(my-chip-8-emulator PRIVATE ${SDL2_CFLAGS_OTHER})