#include "Player.hpp"

Player::Player() {
	blue = Square(0, 0, 0);
	pink = Square(4, 0, 0);

	//to remove
	blue.x = 60;
	pink.x = 180;
}

void Player::update(InputHandler& input_handler, float dt) {
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

	// Update player physics

	//blue.gravity(SQUARE_GRAVITY, SQUARE_GRAVITY_MAX, dt);
	pink.gravity(SQUARE_GRAVITY, SQUARE_GRAVITY_MAX, dt);

	blue.update(dt);
	pink.update(dt);


	// to remove
	if (pink.y > 144) {
		pink.y = 144;
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