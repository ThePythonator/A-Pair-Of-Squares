#include "Door.hpp"

Door::Door() : Entity() {

}

Door::Door(uint16_t sprite_index, float x, float y, uint8_t type) : Entity(sprite_index, x, y) {
	this->type = type;
}

void Door::update(float dt) {
	if (opening && height < GAME::DOOR::OPEN_SIZE) {
		height += GAME::DOOR::OPEN_SPEED * dt;

		if (height > GAME::DOOR::OPEN_SIZE) {
			// Now open
			height = GAME::DOOR::OPEN_SIZE;
			opening = false;
		}
	}
	else if (closing && height > GAME::DOOR::CLOSE_SIZE) {
		height -= GAME::DOOR::CLOSE_SPEED * dt;

		if (height < GAME::DOOR::CLOSE_SIZE) {
			// Now closed
			height = GAME::DOOR::CLOSE_SIZE;
			closing = false;
		}
	}
}

void Door::render(Spritesheet& spritesheet) {
	/*uint8_t i;
	for (i = 0; i < height / SPRITES::SIZE; i++) {
		spritesheet.sprite_scaled(type == 0 ? TILE_ID::DOOR::VERTICAL::BLUE_MIDDLE : TILE_ID::DOOR::VERTICAL::PINK_MIDDLE, x, y - height + GAME::DOOR::CLOSE_SIZE + i * SPRITES::SIZE);
	}*/

	//spritesheet.sprite_scaled(type == 0 ? TILE_ID::DOOR::VERTICAL::BLUE_BOTTOM : TILE_ID::DOOR::VERTICAL::PINK_BOTTOM, x, y - height + GAME::DOOR::CLOSE_SIZE + i * SPRITES::SIZE);

	/*if (height < SPRITES::SIZE) {
		spritesheet.sprite_scaled(type == 0 ? TILE_ID::DOOR::VERTICAL::BLUE_MIDDLE : TILE_ID::DOOR::VERTICAL::PINK_MIDDLE, x, y - height + GAME::DOOR::CLOSE_SIZE);
	}*/
	float clamped = std::max(y - height + GAME::DOOR::CLOSE_SIZE, y - SPRITES::SIZE);

	spritesheet.sprite_scaled(type == 0 ? TILE_ID::DOOR::VERTICAL::BLUE_MIDDLE : TILE_ID::DOOR::VERTICAL::PINK_MIDDLE, x, clamped);
	spritesheet.sprite_scaled(type == 0 ? TILE_ID::DOOR::VERTICAL::BLUE_BOTTOM : TILE_ID::DOOR::VERTICAL::PINK_BOTTOM, x, y - height + GAME::DOOR::CLOSE_SIZE + SPRITES::SIZE);
}

bool Door::check_collision(float player_x, float player_y, float player_w, float player_h) {
	return player_x + player_w > x + GAME::DOOR::BORDER && player_x < x + GAME::DOOR::BORDER + GAME::DOOR::WIDTH && player_y + player_h > y && player_y < get_bottom();
}

float Door::get_bottom() {
	return y - height + GAME::DOOR::CLOSE_SIZE + GAME::DOOR::OPEN_SIZE;
}

bool Door::is_open() {
	return height == GAME::DOOR::OPEN_SIZE;
}

bool Door::is_opening() {
	return opening;
}

void Door::open() {
	if (!is_open()) {
		opening = true;
		closing = false;
	}
}

void Door::close() {
	opening = false;
	closing = true;

	open_timer = 0.0f;
}

uint8_t Door::get_type() {
	return type;
}