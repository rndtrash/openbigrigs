#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include "window.h"
#include "audio.h"
#include "renderer.h"
#include "resources/resource_manager.h"

namespace OBR {

    class Game {
    public:
        Game(int argc, char **argv);

        ~Game();

        void run();

        std::shared_ptr<const AudioSystem> get_audio() { return audio; }

        std::shared_ptr<ResourceManager> get_rm() { return resourceManager; }

        static Game &the() { return *instance; }

    private:
        std::shared_ptr<AudioSystem> audio;
        std::shared_ptr<ResourceManager> resourceManager;
        std::shared_ptr<Renderer> renderer;

        static Game *instance;
    };

}
