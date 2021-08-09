#include "Player.hpp"

Player::Player() {
	// TODO: remove literals ( 0 and 4 )
	blue = Square(0, 0, 0);
	pink = Square(4, 0, 0);
}

Player::Player(uint16_t blue_x, uint16_t blue_y, uint16_t pink_x, uint16_t pink_y) {
	// TODO: remove literals
	blue = Square(0, blue_x, blue_y);
	pink = Square(4, pink_x, pink_y);
}

void Player::update(InputHandler& input_handler, LevelHandler& level_handler, float dt) {
	std::vector<Tile> tiles = level_handler.get_tiles();

	// Handle inputs
	if (KeyHandler::is_down(input_handler.get_key_union().keys.RIGHT)) {
		blue.accelerate(SQUARE_ACCELERATION, SQUARE_VELOCITY_MAX, dt);
		pink.accelerate(-SQUARE_DECELERATION, SQUARE_VELOCITY_MAX, dt);
	}
	else if (KeyHandler::is_down(input_handler.get_key_union().keys.LEFT)) {
		blue.accelerate(-SQUARE_ACCELERATION, SQUARE_VELOCITY_MAX, dt);
		pink.accelerate(SQUARE_DECELERATION, SQUARE_VELOCITY_MAX, dt);
	}
	else {
		// Handle deceleration
		blue.decelerate(SQUARE_DECELERATION, dt);
		pink.decelerate(SQUARE_DECELERATION, dt);
	}

	if (KeyHandler::is_down(input_handler.get_key_union().keys.SPACE)) {
		blue.jump(SQAURE_JUMP_STRENGTH);
		pink.jump(SQAURE_JUMP_STRENGTH);
	}

	// Update player physics
	blue.gravity(SQUARE_GRAVITY, SQUARE_GRAVITY_MAX, dt);
	pink.gravity(SQUARE_GRAVITY, SQUARE_GRAVITY_MAX, dt);

	// Draw players towards finishes if in both are in range
	if (
		std::abs(blue.x - level_handler.level_finish_blue_x) < level_handler.get_sprite_size() &&
		std::abs(blue.y - level_handler.level_finish_blue_y) < level_handler.get_sprite_size() &&
		std::abs(pink.x - level_handler.level_finish_pink_x) < level_handler.get_sprite_size() &&
		std::abs(pink.y - level_handler.level_finish_pink_y) < level_handler.get_sprite_size()) {

		blue.add_velocity(SQAURE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_x - blue.x), SQAURE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_y - blue.y));
		pink.add_velocity(SQAURE_FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_x - pink.x), SQAURE_FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_y - pink.y));
	}

	blue.update(tiles, dt);
	pink.update(tiles, dt);

	// If players are already on finish, keep them locked
	if (std::abs(blue.x - level_handler.level_finish_blue_x) < SQAURE_FINISH_STOP_PULL &&
		std::abs(blue.y - level_handler.level_finish_blue_y) < SQAURE_FINISH_STOP_PULL &&
		std::abs(pink.x - level_handler.level_finish_pink_x) < SQAURE_FINISH_STOP_PULL &&
		std::abs(pink.y - level_handler.level_finish_pink_y) < SQAURE_FINISH_STOP_PULL) {

		blue.x = level_handler.level_finish_blue_x;
		blue.y = level_handler.level_finish_blue_y;
		pink.x = level_handler.level_finish_pink_x;
		pink.y = level_handler.level_finish_pink_y;

		blue.set_finished();
		pink.set_finished();
	}
}

void Player::render(Spritesheet& spritesheet) {
	blue.render(spritesheet);
	pink.render(spritesheet);
}

//void Player::set_spawns(int blue_x, int blue_y, int pink_x, int pink_y) {
//	blue = Square(0, blue_x, blue_y);
//	pink = Square(4, pink_x, pink_y);
//}

float Player::get_blue_x() {
	return blue.x;
}

float Player::get_blue_y() {
	return blue.y;
}

float Player::get_pink_x() {
	return pink.x;
}

float Player::get_pink_y() {
	return pink.y;
}