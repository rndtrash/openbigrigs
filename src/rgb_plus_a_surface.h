#pragma once

#include <SDL3/SDL.h>

#include "resources/surface.h"

namespace OBR {
    class RGBPlusASurface {
    public:
        RGBPlusASurface(SDL_Surface *rgbaSurface, SDL_Surface *alphaSurface);

        ~RGBPlusASurface();

        SDL_Surface *get_surface() { return surface; }

    private:
        SDL_Surface *surface;
    };
}
