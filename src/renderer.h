#pragma once

#include "window.h"

namespace OBR {
    class Renderer {
    public:
        Renderer();

        ~Renderer();

        void render();

    private:
        ObrWindow *window;
    };
}