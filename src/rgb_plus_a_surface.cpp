#include <stdexcept>
#include "rgb_plus_a_surface.h"

namespace OBR {

    RGBPlusASurface::RGBPlusASurface(const SDL_Surface *rgbaSurface, const SDL_Surface *alphaSurface) {
        if (!SDL_RectEquals(&rgbaSurface->clip_rect, &alphaSurface->clip_rect))
            throw std::runtime_error("Attempted to load color and alpha textures for the different cursors");

        surface = SDL_CreateRGBSurfaceWithFormat(0,
                                                 rgbaSurface->clip_rect.w,
                                                 rgbaSurface->clip_rect.h,
                                                       32,
                                                       SDL_PIXELFORMAT_RGBA8888);

        const auto pixelCount = rgbaSurface->clip_rect.w * rgbaSurface->clip_rect.h;
        auto p = (uint8_t *) surface->pixels;
        auto colorP = (uint8_t *) rgbaSurface->pixels;
        auto alphaP = (uint8_t *) alphaSurface->pixels;
        for (auto i = 0; i < pixelCount; i++) {
            // TODO: oh god
            *p++ = *colorP++;
            *p++ = *colorP++;
            *p++ = *colorP++;
            *p++ = *alphaP;
            alphaP += 3;
        }
    }

    RGBPlusASurface::~RGBPlusASurface() {
        SDL_FreeSurface(surface);
    }
}