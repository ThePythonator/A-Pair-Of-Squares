#include "Button.hpp"

Button::Button() : Entity() {

}

Button::Button(uint16_t sprite_index, float x, float y, uint8_t type) : Entity(sprite_index, x, y) {
	this->type = type;
}

void Button::update(float dt) {
	if (release_timer > 0.0f) {
		release_timer -= dt;
		if (release_timer <= 0.0f) {
			release_timer = 0.0f;
			pressed = false;
		}
	}
}

void Button::render(Spritesheet& spritesheet) {
	spritesheet.sprite_scaled(sprite_index + (pressed ? 1 : 0), x, y);
}

void Button::set_pressed(uint8_t type) {
	if (type == this->type) {
		pressed = true;
		release_timer = 0.0f;
	}
}

void Button::set_released(uint8_t type) {
	if (type == this->type && release_timer == 0.0f) {
		release_timer = GAME::BUTTON::RELEASE_DELAY;
	}
}

bool Button::get_pressed() {
	return pressed;
}

bool Button::check_collision(float player_x, float player_y) {
	return player_x < x + GAME::BUTTON::BORDER + GAME::BUTTON::WIDTH && player_x + SPRITES::SIZE > x + GAME::BUTTON::BORDER && player_y < y + SPRITES::SIZE && player_y + SPRITES::SIZE > get_top();
}

bool Button::check_on_top(float player_x, float player_y) {
	return player_x < x + GAME::BUTTON::BORDER + GAME::BUTTON::WIDTH && player_x + SPRITES::SIZE > x + GAME::BUTTON::BORDER && player_y + SPRITES::SIZE == get_top();
}

float Button::get_top() {
	return y + SPRITES::SIZE - (pressed ? GAME::BUTTON::HEIGHT_PRESSED : GAME::BUTTON::HEIGHT_RELEASED);
}

uint8_t Button::get_type() {
	return type;
}