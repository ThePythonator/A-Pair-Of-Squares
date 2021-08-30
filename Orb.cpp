#include "Orb.hpp"

Orb::Orb() : Entity() {

}

Orb::Orb(uint16_t sprite_index, uint8_t type, float x, float y) : Entity(sprite_index, x, y) {
	this->type = type;
}

void Orb::update(float dt) {
	if (collected) {
		fade_age += dt;
	}
}

void Orb::render(Spritesheet& spritesheet) {
	if (collected) {
		if (fade_age < GAME::ORB::FADE::MAX_AGE) {
			// Calculate age ratio
			float age_ratio = (fade_age / GAME::ORB::FADE::MAX_AGE);

			// Get alpha from age
			uint8_t alpha = (1.0f - age_ratio) * 0xFF;

			// Get scale from age
			float scale = age_ratio * GAME::ORB::FADE::MAX_SCALE_BOOST + spritesheet.get_scale();

			// Calculate scale ratio
			float scale_ratio = spritesheet.get_scale() / scale;

			// Get offset to keep sprites centered
			float offset = SPRITES::SIZE * (scale - spritesheet.get_scale()) / 4.0f;
			//float offset = spritesheet.get_sprite_size() * (scale - spritesheet.get_scale()) / 4.0f;
			//float offset = (spritesheet.get_sprite_size() / 2.0f) * (scale - spritesheet.get_scale()) / 2.0f; // Same as line above but less optimized.

			uint8_t old_alpha = spritesheet.get_alpha();
			spritesheet.set_alpha(alpha);
			spritesheet.sprite(sprite_index, (x - offset) * scale_ratio, (y - offset) * scale_ratio, scale, 0.0f, NULL, SDL_FLIP_NONE);
			spritesheet.set_alpha(old_alpha);
		}
	}
	else {
		Entity::render(spritesheet);
	}
}

void Orb::set_collected() {
	collected = true;
}

bool Orb::get_collected() {
	return collected;
}

bool Orb::get_finished() {
	return collected && fade_age >= GAME::ORB::FADE::MAX_AGE;
}

uint8_t Orb::get_type() {
	return type;
}