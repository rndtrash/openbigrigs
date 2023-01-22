#pragma once

#include <memory>

#include <SDL2/SDL.h>

#include "window.h"
#include "audio.h"
#include "fs.h"

namespace OBR
{

class Game
{
public:
	Game(int argc, char** argv);
	~Game();

	void run();

	std::shared_ptr<const AudioSystem> get_audio() { return audio; }
	std::shared_ptr<const FileSystem> get_fs() { return fs; }

	static Game& the() { return *instance; }

private:
	Window* window;

	std::shared_ptr<AudioSystem> audio;
	std::shared_ptr<FileSystem> fs;

	static Game* instance;
};

}
