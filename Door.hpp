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

	bool check_collision(float player_x, float player_y, float player_w, float player_h);

	float get_bottom();

	bool is_open();
	bool is_opening();

	void open();
	void close();

	uint8_t get_type();

protected:
	uint8_t type = 0;

	float height = GAME::DOOR::CLOSE_SIZE;

	bool opening = false;
	bool closing = false;

	float open_timer = 0.0f;
};