#pragma once

#include <cmath>

#include "Constants.hpp"

#include "Square.hpp"
#include "InputManager.hpp"
#include "LevelManager.hpp"
#include "Spritesheet.hpp"
#include "AudioManager.hpp"

class Player {
public:
	Player();
	Player(uint16_t blue_x, uint16_t blue_y, uint16_t pink_x, uint16_t pink_y);

	bool update(InputHandler& input_handler, AudioHandler& audio_handler, LevelHandler& level_handler, float dt);

	void render(Spritesheet& spritesheet);

	void reset_orb_count();

	void reset_players(LevelHandler& level_handler);

	uint8_t get_death_count();
	uint8_t get_orb_count();

	float get_blue_x();
	float get_blue_y();
	float get_pink_x();
	float get_pink_y();

	//void set_spawns(int blue_x, int blue_y, int pink_x, int pink_y);

protected:
	Square blue, pink;

	uint8_t death_count = 0;
	uint8_t orb_count = 0;
};