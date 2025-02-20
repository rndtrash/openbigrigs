#pragma once

#include <SDL2/SDL.h>

#include "resource.h"

namespace OBR {
    class Surface : public Resource {
    public:
        explicit Surface(const std::string &name);

        ~Surface() override;

        SDL_Surface *get_surface() { return surface; }

    private:
        SDL_Surface *surface;
    };
}
