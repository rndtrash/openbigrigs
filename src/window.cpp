#include <stdexcept>

#include <bx/os.h>
#include <bgfx/bgfx.h>
#include <SDL2/SDL_syswm.h>

#include "window.h"

namespace OBR {

    Window::Window(const std::string &title, int width, int height) {
        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
            throw std::runtime_error("Could not initialize SDL Video Subsystem: " + std::string(SDL_GetError()));

        if (SDL_InitSubSystem(SDL_INIT_TIMER) != 0)
            throw std::runtime_error("Could not initialize SDL Timer Subsystem: " + std::string(SDL_GetError()));

        window = SDL_CreateWindow(title.c_str(),
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  width,
                                  height,
                                  SDL_WINDOW_SHOWN);
        if (window == nullptr)
            throw std::runtime_error("Failed creating window: " + std::string(SDL_GetError()));

        SDL_ShowWindow(window);

        bgfx::Init init;

        init.type = bgfx::RendererType::Count;
        init.resolution.width = width;
        init.resolution.height = height;
        init.resolution.reset = BGFX_RESET_VSYNC; // TODO: depend on flags passed in the constructor

        SDL_SysWMinfo wmi;
        SDL_VERSION(&wmi.version);
        if (!SDL_GetWindowWMInfo(window, &wmi))
            throw std::runtime_error("Failed getting info about window: " + std::string(SDL_GetError()));

#	if BX_PLATFORM_LINUX
        // TODO: wayland
        {
            init.platformData.nwh = (void *) wmi.info.x11.window;
            init.platformData.ndt = wmi.info.x11.display;
            init.platformData.type = bgfx::NativeWindowHandleType::Default;
        }
#	else
#	    if BX_PLATFORM_OSX || BX_PLATFORM_IOS
        init.platformData.nwh = wmi.info.cocoa.window;
#	    elif BX_PLATFORM_WINDOWS
        init.platformData.nwh = wmi.info.win.window;
#       elif BX_PLATFORM_ANDROID
        init.platformData.nwh = wmi.info.android.window;
#	    endif // BX_PLATFORM_

        init.platformData.ndt = nullptr;
        init.platformData.type = bgfx::NativeWindowHandleType::Default;
#	endif // BX_PLATFORM_*

        bgfx::init(init);

        bgfx::setDebug(BGFX_DEBUG_TEXT); // TODO: remove it

        bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
    }

    Window::~Window() {
        SDL_DestroyWindow(window);
    }

    std::pair<int, int> Window::get_size() {
        int width = -1, height = -1;
        SDL_GetWindowSize(window, &width, &height);

        return std::make_pair(width, height);
    }

}
