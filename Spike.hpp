#pragma once

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

	} direction;

	Spike();
	Spike(uint8_t sprite_index, SpikeDirection direction, float x, float y);

	//void update(std::vector<Tile>& tiles, float dt);

	void render(Spritesheet& spritesheet);
	//void render(Spritesheet& spritesheet, Camera& camera);

protected:
};