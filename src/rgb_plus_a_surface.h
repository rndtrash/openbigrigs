#pragma once

#include <SDL2/SDL.h>

#include "resources/surface.h"

namespace OBR {
    class RGBPlusASurface {
    public:
        RGBPlusASurface(const SDL_Surface *rgbaSurface, const SDL_Surface *alphaSurface);

        ~RGBPlusASurface();

        SDL_Surface *get_surface() { return surface; }

    private:
        SDL_Surface *surface;
    };
}
