#include <stdexcept>

#include <SDL3/SDL.h>
#include <bx/os.h>
#include <bgfx/bgfx.h>
#include <X11/Xlib.h>

#include "window.h"
#include "game.h"

namespace OBR {

    ObrWindow::ObrWindow(const std::string &title, int width, int height) {
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
            throw std::runtime_error("Could not initialize SDL Video Subsystem: " + std::string(SDL_GetError()));

        // window = SDL_CreateWindow(title.c_str(),
        //                           SDL_WINDOWPOS_UNDEFINED,
        //                           SDL_WINDOWPOS_UNDEFINED,
        //                           width,
        //                           height,
        //                           SDL_WINDOW_SHOWN);
        window = SDL_CreateWindow(title.c_str(), width, height, 0);
        if (window == nullptr)
            throw std::runtime_error("Failed creating window: " + std::string(SDL_GetError()));

        SDL_ShowWindow(window);

        {
            auto cursorColor = Game::the().get_rm()->surfaces->get("Data/Menu/cursor.bmp");
            auto cursorColorSurface = cursorColor->get_surface();
            auto cursorAlpha = Game::the().get_rm()->surfaces->get("Data/Menu/cursor_a.bmp");
            auto cursorAlphaSurface = cursorAlpha->get_surface();

            cursorSurface = std::make_unique<RGBPlusASurface>(cursorColorSurface, cursorAlphaSurface);
        }
        spdlog::debug("cursorColor and cursorAlpha should be destroyed by now");

        // TODO: the hotspot is probably defined somewhere
        cursor = SDL_CreateColorCursor(cursorSurface->get_surface(), 0, 0);
        if (cursor == nullptr) {
            cursorSurface.reset();
            throw std::runtime_error("Could not create a cursor: " + std::string(SDL_GetError()));
        }

        SDL_SetCursor(cursor);

        bgfx::Init init;

        init.type = bgfx::RendererType::Count;
        init.resolution.width = width;
        init.resolution.height = height;
        init.resolution.reset = BGFX_RESET_VSYNC; // TODO: depend on flags passed in the constructor

#	if BX_PLATFORM_LINUX
        if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
            auto *xdisplay = static_cast<Display*>(
                SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr));
            auto xwindow = static_cast<Window>(SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
            if (xdisplay && xwindow) {
                init.platformData.nwh = reinterpret_cast<void*>(xwindow);
                init.platformData.ndt = xdisplay;
                init.platformData.type = bgfx::NativeWindowHandleType::Default;
            }
        } else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
            struct wl_display *display = (struct wl_display *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, NULL);
            struct wl_surface *surface = (struct wl_surface *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, NULL);
            if (display && surface) {
                // TODO: wayland
            }
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

    ObrWindow::~ObrWindow() {
        SDL_DestroyCursor(cursor);
        cursorSurface.reset();

        SDL_DestroyWindow(window);
    }

    std::pair<int, int> ObrWindow::get_size() {
        int width = -1, height = -1;
        SDL_GetWindowSize(window, &width, &height);

        return std::make_pair(width, height);
    }

}
