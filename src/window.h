#pragma once

#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include "rgb_plus_a_surface.h"

namespace OBR {

    class ObrWindow {
    public:
        ObrWindow(const std::string &title, int width, int height); // TODO: accept flags for turning VSync on and off
        ~ObrWindow();

        SDL_Window *get_window() { return window; }

        /// Get dimensions of the window
        /// \returns std::pair with width and height of the window
        std::pair<int, int> get_size();

    private:
        SDL_Window *window = nullptr;

        std::unique_ptr<RGBPlusASurface> cursorSurface;
        SDL_Cursor *cursor = nullptr;
    };

}
