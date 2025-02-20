#include <stdexcept>
#include "rgb_plus_a_surface.h"

namespace OBR
{
    RGBPlusASurface::RGBPlusASurface(SDL_Surface* rgbaSurface, SDL_Surface* alphaSurface)
    {
        SDL_Rect rgbaRect, alphaRect;
        if (!SDL_GetSurfaceClipRect(rgbaSurface, &rgbaRect) || !SDL_GetSurfaceClipRect(alphaSurface, &alphaRect))
            throw std::runtime_error("Failed to get the size of the RGBA surfaces: " + std::string(SDL_GetError()));

        if (!SDL_RectsEqual(&rgbaRect, &alphaRect))
            throw std::runtime_error("Attempted to load color and alpha textures for the different cursors");

        surface = SDL_CreateSurface(rgbaRect.w,
                                    rgbaRect.h,
                                    SDL_PIXELFORMAT_RGBA8888);

        const auto pixelCount = rgbaRect.w * rgbaRect.h;
        auto p = static_cast<uint8_t*>(surface->pixels);
        auto colorP = static_cast<uint8_t*>(rgbaSurface->pixels);
        auto alphaP = static_cast<uint8_t*>(alphaSurface->pixels);
        for (auto i = 0; i < pixelCount; i++)
        {
            // TODO: oh god
            *p++ = *colorP++;
            *p++ = *colorP++;
            *p++ = *colorP++;
            *p++ = *alphaP;
            alphaP += 3;
        }
    }

    RGBPlusASurface::~RGBPlusASurface()
    {
        SDL_DestroySurface(surface);
    }
}
