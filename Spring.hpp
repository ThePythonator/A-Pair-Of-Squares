#pragma once

#include "Constants.hpp"

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Spring : public Entity {
public:
	Spring();
	Spring(uint16_t sprite_index, float x, float y);

	void update(float dt);//Player& player,

	void render(Spritesheet& spritesheet);

	bool should_launch();

	bool check_collision(float player_x, float player_y);
	bool check_on_top(float  player_x, float player_y);

	float get_top();

protected:
	float animation_timer = 0.0f;

	uint8_t current_frame_offset = 0;

	bool launch_player = false;
};