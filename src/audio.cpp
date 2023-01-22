#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <spdlog/spdlog.h>

#include "audio.h"

namespace OBR
{

AudioSystem::AudioSystem()
{
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error("Could not initialize SDL Audio Subsystem: " + std::string(SDL_GetError()));

	if (Mix_OpenAudioDevice(44100, AUDIO_S16SYS, 2, 2048, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE) != 0) // some reasonable default according to the SDL Wiki
		throw std::runtime_error("Could not find a suitable audio device: " + std::string(SDL_GetError()));
}

AudioSystem::~AudioSystem()
{
	Mix_CloseAudio(); // TODO: what did he say about unfreed chunks? should i care?
	Mix_Quit();
}

}
