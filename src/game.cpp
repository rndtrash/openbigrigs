#include <stdexcept>

#include <bgfx/bgfx.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

#include "game.h"

namespace OBR {

    Game *Game::instance = nullptr;

    Game::Game(int argc, char **argv) {
        instance = this;

        if (SDL_WasInit(0) == 0) {
            SDL_SetMainReady();
            if (!SDL_Init(0))
                throw std::runtime_error("Could not initialize SDL: " + std::string(SDL_GetError()));
        }

        resourceManager = std::make_shared<ResourceManager>(argc, argv);
        audio = std::make_shared<AudioSystem>();
        renderer = std::make_shared<Renderer>();
    }

    Game::~Game() {
        renderer.reset();
        audio.reset(); // explicitly freeing before the SDL_Quit

        SDL_Quit();
    }

    void Game::run() {
        SDL_Event event;
        bool running = true;
        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT) running = false;
            }

            renderer->render();

            bgfx::frame();
        }
    }

}
