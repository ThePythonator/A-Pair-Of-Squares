#include "Orb.hpp"

Orb::Orb() : Entity() {

}

Orb::Orb(uint8_t sprite_index, uint8_t type, float x, float y) : Entity(sprite_index, x, y) {
	this->type = type;
}

void Orb::render(Spritesheet& spritesheet) {
	if (!collected) {
		//spritesheet.sprite_scaled(sprite_index, x, y);
		Entity::render(spritesheet);
	}
}

void Orb::set_collected() {
	collected = true;
}

bool Orb::get_collected() {
	return collected;
}

uint8_t Orb::get_type() {
	return type;
}