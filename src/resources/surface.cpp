#include <spdlog/spdlog.h>
#include <SDL_image.h>

#include "surface.h"

namespace OBR {
    Surface::Surface(const std::string &name) : Resource(name) {
        const auto size = get_file_size();
        std::unique_ptr<uint8_t[]> buffer(new uint8_t[size]);
        read_file_to(buffer.get(), size);
        auto stream = SDL_RWFromConstMem(buffer.get(), size);

        if (name.ends_with(".bmp"))
            surface = SDL_LoadBMP_RW(stream, true);
        else
            surface = IMG_Load_RW(stream, true);

        if (surface == nullptr)
            throw std::runtime_error("Could not create a cursor color surface: " + std::string(SDL_GetError()));
    }

    Surface::~Surface() {
        SDL_FreeSurface(surface);
    }
}