#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include "window.h"
#include "audio.h"

namespace OBR
{

class Game
{
public:
	Game();
	~Game();

	void run();

	std::shared_ptr<const AudioSystem> get_audio() { return audio; }

	static Game& the() { return *instance; }

private:
	Window* window;

	std::shared_ptr<AudioSystem> audio;

	static Game* instance;
};

}
