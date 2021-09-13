#include "Button.hpp"

Button::Button() : Entity() {

}

Button::Button(uint16_t sprite_index, float x, float y) : Entity(sprite_index, x, y) {

}

void Button::update(float dt) {

}

void Button::render(Spritesheet& spritesheet) {
	spritesheet.sprite_scaled(sprite_index, x, y);
}

void Button::set_pressed(bool pressed) {
	this->pressed = pressed;
}

bool Button::get_pressed() {
	return pressed;
}

bool Button::check_collision(float player_x, float player_y) {
	return player_x < x + GAME::BUTTON::BORDER + GAME::BUTTON::WIDTH && player_x + SPRITES::SIZE > x + GAME::BUTTON::BORDER && player_y < y + SPRITES::SIZE && player_y + SPRITES::SIZE > get_top();
}

bool Button::check_on_top(float player_x, float player_y){
	return player_x < x + GAME::BUTTON::BORDER + GAME::BUTTON::WIDTH && player_x + SPRITES::SIZE > x + GAME::BUTTON::BORDER && player_y + SPRITES::SIZE == get_top();
}

float Button::get_top() {
	return y + SPRITES::SIZE - pressed ? GAME::BUTTON::HEIGHT_PRESSED : GAME::BUTTON::HEIGHT_RELEASED; // check works
}