#pragma once

#include "Constants.hpp"

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Spike : public Entity {
public:
	enum class SpikeDirection {
		DOUBLE_BOTTOM,
		DOUBLE_LEFT,
		DOUBLE_TOP,
		DOUBLE_RIGHT,

		SINGLE_BOTTOM_LEFT,
		SINGLE_BOTTOM_RIGHT,

		SINGLE_LEFT_TOP,
		SINGLE_LEFT_BOTTOM,

		SINGLE_TOP_LEFT,
		SINGLE_TOP_RIGHT,

		SINGLE_RIGHT_TOP,
		SINGLE_RIGHT_BOTTOM

		/*BOTTOM = 0b0000,
		TOP = 0b0001,
		LEFT = 0b0010,
		RIGHT = 0b0100,

		SINGLE = 0b0000,
		DOUBLE = 0b1000,

		DOUBLE_BOTTOM = DOUBLE | BOTTOM,
		DOUBLE_LEFT = DOUBLE | LEFT,
		DOUBLE_TOP = DOUBLE | TOP,
		DOUBLE_RIGHT = DOUBLE | RIGHT,

		SINGLE_BOTTOM_LEFT = SINGLE | BOTTOM | LEFT,
		SINGLE_BOTTOM_RIGHT = SINGLE | BOTTOM | RIGHT,

		SINGLE_LEFT_TOP = SINGLE | LEFT | TOP,
		SINGLE_LEFT_BOTTOM = SINGLE | LEFT | BOTTOM,

		SINGLE_TOP_LEFT = SINGLE | TOP | LEFT,
		SINGLE_TOP_RIGHT = SINGLE | TOP | RIGHT,

		SINGLE_RIGHT_TOP = SINGLE | RIGHT | TOP,
		SINGLE_RIGHT_BOTTOM = SINGLE | RIGHT | BOTTOM*/

		// Doesn't matter that SINGLE_TOP_RIGHT is same as SINGLE_RIGHT_TOP etc, since they occupy the same quadrant

	} direction;

	Spike();
	Spike(uint16_t sprite_index, SpikeDirection direction, float x, float y);

	//void update(std::vector<Tile>& tiles, float dt);

	void render(Spritesheet& spritesheet);
	//void render(Spritesheet& spritesheet, Camera& camera);

	bool check_collision(float player_x, float player_y);

protected:
};