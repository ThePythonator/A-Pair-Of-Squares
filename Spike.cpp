#include "Spike.hpp"

Spike::Spike() : Entity() {
	direction = SpikeDirection::DOUBLE_BOTTOM;
}

Spike::Spike(uint16_t sprite_index, SpikeDirection direction, float x, float y) : Entity(sprite_index, x, y) {
	this->direction = direction;
}

void Spike::render(Spritesheet& spritesheet) {
	Entity::render(spritesheet);
}

bool Spike::check_collision(float player_x, float player_y) {
	uint16_t offset_x, offset_y, w, h;

	/*if ((uint8_t)direction & (uint8_t)SpikeDirection::LEFT || ((uint8_t)direction & (uint8_t)SpikeDirection::DOUBLE && ((uint8_t)direction & (uint8_t)SpikeDirection::TOP || (uint8_t)direction & (uint8_t)SpikeDirection::BOTTOM))) {

	}*/

	switch (direction) {
	case Spike::SpikeDirection::DOUBLE_BOTTOM:
		offset_x = 0;
		offset_y = SPRITES::SIZE_HALF;
		w = SPRITES::SIZE;
		h = SPRITES::SIZE_HALF;
		break;

	case Spike::SpikeDirection::DOUBLE_LEFT:
		offset_x = 0;
		offset_y = 0;
		w = SPRITES::SIZE_HALF;
		h = SPRITES::SIZE;
		break;

	case Spike::SpikeDirection::DOUBLE_TOP:
		offset_x = 0;
		offset_y = 0;
		w = SPRITES::SIZE;
		h = SPRITES::SIZE_HALF;
		break;

	case Spike::SpikeDirection::DOUBLE_RIGHT:
		offset_x = SPRITES::SIZE_HALF;
		offset_y = 0;
		w = SPRITES::SIZE_HALF;
		h = SPRITES::SIZE;
		break;

	case Spike::SpikeDirection::SINGLE_BOTTOM_LEFT:
	case Spike::SpikeDirection::SINGLE_LEFT_BOTTOM:
		offset_x = 0;
		offset_y = SPRITES::SIZE_HALF;
		w = SPRITES::SIZE_HALF;
		h = SPRITES::SIZE_HALF;
		break;

	case Spike::SpikeDirection::SINGLE_LEFT_TOP:
	case Spike::SpikeDirection::SINGLE_TOP_LEFT:
		offset_x = 0;
		offset_y = 0;
		w = SPRITES::SIZE_HALF;
		h = SPRITES::SIZE_HALF;
		break;

	case Spike::SpikeDirection::SINGLE_BOTTOM_RIGHT:
	case Spike::SpikeDirection::SINGLE_RIGHT_BOTTOM:
		offset_x = SPRITES::SIZE_HALF;
		offset_y = SPRITES::SIZE_HALF;
		w = SPRITES::SIZE_HALF;
		h = SPRITES::SIZE_HALF;
		break;

	case Spike::SpikeDirection::SINGLE_RIGHT_TOP:
	case Spike::SpikeDirection::SINGLE_TOP_RIGHT:
		offset_x = SPRITES::SIZE_HALF;
		offset_y = 0;
		w = SPRITES::SIZE_HALF;
		h = SPRITES::SIZE_HALF;
		break;

	default:
		// Should never happen!
		offset_x = offset_y = w = h = 0;
	}

	// Allow a tolerance for collisions

	return player_x + GAME::SQUARE::WIDTH > x + offset_x + GAME::SPIKE::TOLERANCE && player_x < x + offset_x + w - GAME::SPIKE::TOLERANCE &&
		player_y + GAME::SQUARE::HEIGHT > y + offset_y + GAME::SPIKE::TOLERANCE && player_y < y + offset_y + h - GAME::SPIKE::TOLERANCE;
}