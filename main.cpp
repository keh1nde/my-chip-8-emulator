//
// Created by Kehinde Adeoso on 8/16/25.
//

#include "CHIP8.h"

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640, 320, // Scaled: 64x32 -> 640x320
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    CHIP8Context chip8;
    chip8.CPUReset();

    bool running = true;
    SDL_Event e;

    const int INSTRUCTIONS_PER_SECOND = 900; // Can be adjusted to be between 500-1000
    const int INSTRUCTIONS_PER_FRAME = INSTRUCTIONS_PER_SECOND / 60;

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        double elapsedMs = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();

        if (elapsedMs >= (1000.0 / 60.0)) { // 60Hz tick
            lastTime = currentTime;

            // Handle input events once per frame
            chip8.processInput(chip8, e, running);

            // Run one CPU cycle
            for (int i = 0; i < INSTRUCTIONS_PER_FRAME; i++) {
                chip8.execute();

                if (chip8.m_DelayTimer > 0) {
                    chip8.m_DelayTimer--;
                }

                if (chip8.m_SoundTimer > 0) {
                    chip8.m_SoundTimer--;
                    if (chip8.m_SoundTimer == 0) {
                        // TODO: Stop Sound.
                    }
                }
            }
            chip8.render(renderer);
            // SDL_Delay(16); (Delay to simulate ~60Hz)
            SDL_Delay(1);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

