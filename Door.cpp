#include "Door.hpp"

Door::Door() : Entity() {

}

Door::Door(uint16_t sprite_index, float x, float y, uint8_t type) : Entity(sprite_index, x, y) {
	this->type = type;
}

void Door::update(float dt) {

}

void Door::render(Spritesheet& spritesheet) {
	//Entity::render(spritesheet);
}

uint8_t Door::get_type() {
	return type;
}