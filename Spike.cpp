#include "Spike.hpp"

Spike::Spike() : Entity() {

}

Spike::Spike(uint8_t sprite_index, SpikeDirection direction, float x, float y) : Entity(sprite_index, x, y) {
	this->direction = direction;
}

void Spike::render(Spritesheet& spritesheet) {
	Entity::render(spritesheet);
}

bool Spike::check_collision(float player_x, float player_y, uint8_t sprite_size) {
	uint16_t offset_x, offset_y, w, h;

	uint8_t sprite_size_half = sprite_size / 2;

	/*if ((uint8_t)direction & (uint8_t)SpikeDirection::LEFT || ((uint8_t)direction & (uint8_t)SpikeDirection::DOUBLE && ((uint8_t)direction & (uint8_t)SpikeDirection::TOP || (uint8_t)direction & (uint8_t)SpikeDirection::BOTTOM))) {

	}*/

	switch (direction) {
	case Spike::SpikeDirection::DOUBLE_BOTTOM:
		offset_x = 0;
		offset_y = sprite_size_half;
		w = sprite_size;
		h = sprite_size_half;
		break;

	case Spike::SpikeDirection::DOUBLE_LEFT:
		offset_x = 0;
		offset_y = 0;
		w = sprite_size_half;
		h = sprite_size;
		break;

	case Spike::SpikeDirection::DOUBLE_TOP:
		offset_x = 0;
		offset_y = 0;
		w = sprite_size;
		h = sprite_size_half;
		break;

	case Spike::SpikeDirection::DOUBLE_RIGHT:
		offset_x = sprite_size_half;
		offset_y = 0;
		w = sprite_size_half;
		h = sprite_size;
		break;

	case Spike::SpikeDirection::SINGLE_BOTTOM_LEFT:
	case Spike::SpikeDirection::SINGLE_LEFT_BOTTOM:
		offset_x = 0;
		offset_y = sprite_size_half;
		w = sprite_size_half;
		h = sprite_size_half;
		break;

	case Spike::SpikeDirection::SINGLE_LEFT_TOP:
	case Spike::SpikeDirection::SINGLE_TOP_LEFT:
		offset_x = 0;
		offset_y = 0;
		w = sprite_size_half;
		h = sprite_size_half;
		break;

	case Spike::SpikeDirection::SINGLE_BOTTOM_RIGHT:
	case Spike::SpikeDirection::SINGLE_RIGHT_BOTTOM:
		offset_x = sprite_size_half;
		offset_y = sprite_size_half;
		w = sprite_size_half;
		h = sprite_size_half;
		break;

	case Spike::SpikeDirection::SINGLE_RIGHT_TOP:
	case Spike::SpikeDirection::SINGLE_TOP_RIGHT:
		offset_x = sprite_size_half;
		offset_y = 0;
		w = sprite_size_half;
		h = sprite_size_half;
		break;

	default:
		// Should never happen!
		offset_x = offset_y = w = h = 0;
	}

	return player_x + sprite_size > x + offset_x && player_x < x + offset_x + w &&
		player_y + sprite_size > y + offset_y && player_y < y + offset_y + h;
}