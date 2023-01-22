#include <stdexcept>

#include <bgfx/bgfx.h>
#include <SDL2/SDL_syswm.h>

#include "window.h"

namespace OBR
{

Window::Window(const std::string& title, int width, int height)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error("Could not initialize SDL Video Subsystem: " + std::string(SDL_GetError()));

	// TODO: do i *really* need a timer?
	// if (SDL_InitSubSystem(SDL_INIT_TIMER) != 0)
		// throw std::runtime_error("Could not initialize SDL Timer Subsystem: " + std::string(SDL_GetError()));

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

#if defined(_WIN32)
	init.platformData.nwh = wmi.info.win.window;
#elif 1
	// TODO: platform-specific shit ahead!
	init.platformData.ndt = wmi.info.x11.display;
	init.platformData.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#else
	#error "Your platform's data getter is not implemented!"
#endif
	
	bgfx::init(init);

	bgfx::setDebug(BGFX_DEBUG_TEXT); // TODO: remove it

	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
}

Window::~Window()
{
	SDL_DestroyWindow(window);
}

}
