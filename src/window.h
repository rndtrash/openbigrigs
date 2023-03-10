#pragma once

#include <string>

#include <SDL2/SDL.h>

namespace OBR
{

class Window
{
public:
	Window(const std::string& title, int width, int height); // TODO: accept flags for turning VSync on and off
	~Window();

	SDL_Window* get_window() { return window; }

private:
	SDL_Window* window = nullptr;
};

}
