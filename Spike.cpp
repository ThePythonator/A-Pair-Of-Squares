#include "Spike.hpp"

Spike::Spike() : Entity() {

}

Spike::Spike(uint8_t sprite_index, SpikeDirection direction, float x, float y) : Entity(sprite_index, x, y) {
	this->direction = direction;
}

void Spike::render(Spritesheet& spritesheet) {
	Entity::render(spritesheet);
}