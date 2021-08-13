#include "Star.hpp"

Star::Star() : Entity() {

}

Star::Star(uint8_t sprite_index, float x, float y) : Entity(sprite_index, x, y) {

}

void Star::render(Spritesheet& spritesheet) {
	if (!collected) {
		//spritesheet.sprite_scaled(sprite_index, x, y);
		Entity::render(spritesheet);
	}
}

void Star::set_collected() {
	collected = true;
}