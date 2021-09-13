#pragma once

#include "Constants.hpp"

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Button : public Entity {
public:
	Button();
	Button(uint16_t sprite_index, float x, float y);

	void update(float dt);

	void render(Spritesheet& spritesheet);

	void set_pressed(bool pressed);
	bool get_pressed();

	bool check_collision(float player_x, float player_y);
	bool check_on_top(float  player_x, float player_y);

	float get_top();

protected:
	bool pressed = false;
};