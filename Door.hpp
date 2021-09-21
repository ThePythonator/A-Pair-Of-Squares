#pragma once

#include "Constants.hpp"

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Door : public Entity {
public:
	Door();
	Door(uint16_t sprite_index, float x, float y, uint8_t type);

	void update(float dt);

	void render(Spritesheet& spritesheet);

	bool check_collision(float player_x, float player_y);
	bool check_on_top(float  player_x, float player_y);

	float get_top();

	uint8_t get_type();

protected:
	bool collected = false;

	uint8_t type = 0;
};