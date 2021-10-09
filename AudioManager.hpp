#pragma once

#include "SDL_mixer.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "Constants.hpp"

typedef Mix_Chunk* Sound;
typedef Mix_Music* Music;

class AudioHandler {
public:
	AudioHandler();

	bool init(uint8_t channels = AUDIO::DEFAULT_CHANNELS, int frequency = AUDIO::DEFAULT_FREQUENCY, int chunksize = AUDIO::DEFAULT_CHUNKSIZE, uint16_t format = MIX_DEFAULT_FORMAT);

	void quit();

	// todo: add loading/clearing funcs and maybe typedef Mix_Chunk
	
	uint8_t play_sound(Sound sample, int loops = 0, int channel = -1);

	void set_sound_volume(float volume);
	void set_sound_volume(uint8_t channel, float volume);
	float get_sound_volume(uint8_t channel);

	// Can be wav or ogg
	static Sound load_sound(std::string path);
	static void free_sound(Sound* sample);

	void play_music(Music music, int loops = -1);
	void halt_music();

	bool is_music_playing();
	bool is_music_fading_in();
	bool is_music_fading_out();

	void fade_music_in(Music music, float fade_time, int loops = -1);
	void fade_music_out(Music music, float fade_time, int loops = -1);

	void set_music_volume(float volume);
	float get_music_volume();

	// Can be wav, midi, ogg or flac
	static Music load_music(std::string path);
	static void free_music(Music* sample);

	std::vector<Sound> sound_samples;
	std::vector<Music> music_samples; // maybe move to private and in load sound/music, auto append to these and return index of item added, then play sound by index?

private:
	static int convert_volume_float_to_int(float volume);
	static float convert_volume_int_to_float(int volume);

	static int convert_s_to_ms_float_to_int(float s);

	int frequency = AUDIO::DEFAULT_FREQUENCY;
	uint16_t format = MIX_DEFAULT_FORMAT;
	uint8_t channels = AUDIO::DEFAULT_CHANNELS;
	int chunksize = AUDIO::DEFAULT_CHUNKSIZE;
};