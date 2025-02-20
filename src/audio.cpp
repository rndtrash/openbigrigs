#include <string>
#include <stdexcept>

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <spdlog/spdlog.h>

#include "audio.h"

namespace OBR
{
    AudioSystem::AudioSystem()
    {
        if (!SDL_InitSubSystem(SDL_INIT_AUDIO))
            throw std::runtime_error("Could not initialize SDL Audio Subsystem: " + std::string(SDL_GetError()));

        const SDL_AudioSpec spec = {SDL_AUDIO_S16, 2, 44100}; // some reasonable default according to the SDL Wiki
        const auto audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
        if (audioStream == nullptr)
            throw std::runtime_error("Could not find a suitable audio device: " + std::string(SDL_GetError()));

        SDL_ResumeAudioDevice(SDL_GetAudioStreamDevice(audioStream));
    }

    AudioSystem::~AudioSystem()
    {
        Mix_CloseAudio(); // TODO: what did valgrind say about unfreed chunks? should i care?
        Mix_Quit();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }
}
