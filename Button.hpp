#pragma once

#include "Constants.hpp"

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Button : public Entity {
public:
	Button();
	Button(uint16_t sprite_index, float x, float y, uint8_t type);

	void update(float dt);

	void render(Spritesheet& spritesheet);

	void set_pressed(uint8_t type);
	void set_released(uint8_t type);
	bool get_pressed();

	bool check_collision(float player_x, float player_y);
	bool check_on_top(float  player_x, float player_y);

	float get_top();

	uint8_t get_type();

protected:
	bool pressed = false;

	uint8_t type = 0;

	float release_timer = 0.0f;
};