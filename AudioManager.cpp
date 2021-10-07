#include "AudioManager.hpp"

AudioHandler::AudioHandler() {

}

bool AudioHandler::init(int frequency, uint16_t format, uint8_t channels, int chunksize) {
	if (channels > MIX_CHANNELS) {
		printf("Too many channels for SDL_mixer (%u), defaulting to maximum (%u).\n", channels, MIX_CHANNELS);
		channels = MIX_CHANNELS;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	this->frequency = frequency;
	this->format = format;
	this->channels = channels;
	this->chunksize = chunksize;

	return true;
}

uint8_t AudioHandler::play(Mix_Chunk* chunk, int loops, int channel) {
	return Mix_PlayChannel(channel, chunk, loops);
}