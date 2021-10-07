#pragma once

#include "SDL_mixer.h"

#include <cstdint>
#include <cstdio>

#include "Constants.hpp"

class AudioHandler {
public:
	AudioHandler();

	bool init(int frequency = AUDIO::DEFAULT_FREQUENCY, uint16_t format = MIX_DEFAULT_FORMAT, uint8_t channels = AUDIO::DEFAULT_CHANNELS, int chunksize = AUDIO::DEFAULT_CHUNKSIZE);

	// todo: add loading/clearing funcs and maybe typedef Mix_Chunk
	
	uint8_t play(Mix_Chunk* chunk, int loops = 0, int channel = -1);

private:
	int frequency = AUDIO::DEFAULT_FREQUENCY;
	uint16_t format = MIX_DEFAULT_FORMAT;
	uint8_t channels = AUDIO::DEFAULT_CHANNELS;
	int chunksize = AUDIO::DEFAULT_CHUNKSIZE;
};