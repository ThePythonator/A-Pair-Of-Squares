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
		blue.jump(SQUARE_JUMP_STRENGTH);
		pink.jump(SQUARE_JUMP_STRENGTH);
	}

	// Update player physics
	blue.gravity(SQUARE_GRAVITY, SQUARE_GRAVITY_MAX, dt);
	pink.gravity(SQUARE_GRAVITY, SQUARE_GRAVITY_MAX, dt);

	// Draw players towards finishes if in both are in range
	if (std::abs(blue.get_x() - level_handler.level_finish_blue_x) < level_handler.get_sprite_size() &&
		std::abs(blue.get_y() - level_handler.level_finish_blue_y) < level_handler.get_sprite_size() / 2 &&
		std::abs(pink.get_x() - level_handler.level_finish_pink_x) < level_handler.get_sprite_size() &&
		std::abs(pink.get_y() - level_handler.level_finish_pink_y) < level_handler.get_sprite_size() / 2 &&
		!blue.get_finished() && !pink.get_finished()) {

		//float blue_vel_x = SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_x - blue.get_x());
		//float blue_vel_y = 0.0f;//SQAURE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_y - blue.get_y());

		//float pink_vel_x = SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_x - pink.get_x());
		//float pink_vel_y = 0.0f;//SQUARE_FINISH_PULL_MIN_VELOCITY * (level_handler.level_finish_pink_y - pink.get_y());

		/*if (std::abs(blue_vel_x) < SQUARE_FINISH_PULL_MIN_VELOCITY) {
			blue_vel_x = SQUARE_FINISH_PULL_MIN_VELOCITY * blue_vel_x > 0 ? 1 : -1;
		}
		if (std::abs(pink_vel_x) < SQUARE_FINISH_PULL_MIN_VELOCITY) {
			pink_vel_x = SQUARE_FINISH_PULL_MIN_VELOCITY * pink_vel_x > 0 ? 1 : -1;
		}*/

		//blue.add_velocity(blue_vel_x, blue_vel_y);
		//pink.add_velocity(pink_vel_x, pink_vel_y);

		//blue.add_velocity(SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_x - blue.get_x() > 0 ? 1 : -1), 0.0f);
		//pink.add_velocity(SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_x - pink.get_x() > 0 ? 1 : -1), 0.0f);

		float blue_vel_x = SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_x - blue.get_x());
		float blue_vel_y = SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_y - blue.get_y());

		float pink_vel_x = SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_x - pink.get_x());
		float pink_vel_y = SQUARE_FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_y - pink.get_y());

		if (std::abs(blue_vel_x) < SQUARE_FINISH_PULL_MIN_VELOCITY) {
			blue_vel_x = SQUARE_FINISH_PULL_MIN_VELOCITY * blue_vel_x > 0 ? 1 : -1;
		}
		if (std::abs(pink_vel_x) < SQUARE_FINISH_PULL_MIN_VELOCITY) {
			pink_vel_x = SQUARE_FINISH_PULL_MIN_VELOCITY * pink_vel_x > 0 ? 1 : -1;
		}

		blue.add_velocity(blue_vel_x, blue_vel_y);
		pink.add_velocity(pink_vel_x, pink_vel_y);
	}

	blue.update(tiles, dt);
	pink.update(tiles, dt);

	// If players are already on finish, keep them locked
	if (std::abs(blue.get_x() - level_handler.level_finish_blue_x) < SQUARE_FINISH_MIN_DISTANCE &&
		std::abs(blue.get_y() - level_handler.level_finish_blue_y) < SQUARE_FINISH_MIN_DISTANCE &&
		std::abs(pink.get_x() - level_handler.level_finish_pink_x) < SQUARE_FINISH_MIN_DISTANCE &&
		std::abs(pink.get_y() - level_handler.level_finish_pink_y) < SQUARE_FINISH_MIN_DISTANCE) {

		blue.set_x(level_handler.level_finish_blue_x);
		blue.set_y(level_handler.level_finish_blue_y);
		pink.set_x(level_handler.level_finish_pink_x);
		pink.set_y(level_handler.level_finish_pink_y);

		blue.set_finished();
		pink.set_finished();
	}

	// Handle orb collisions
	orb_count += level_handler.handle_orb_collisions(blue.get_x(), blue.get_y(), 0);
	orb_count += level_handler.handle_orb_collisions(pink.get_x(), pink.get_y(), 1);

	// Handle spike collisions
	if (level_handler.handle_spike_collisions(blue.get_x(), blue.get_y())) {
		// Blue hit spikes
		// Reset position
		reset_player_positions(level_handler);
		
		// Add to death count
		death_count++;
		
	}
	else if (level_handler.handle_spike_collisions(pink.get_x(), pink.get_y())) {
		// Pink hit spikes
		// Reset position
		reset_player_positions(level_handler);

		// Add to death count
		death_count++;
	}
}

void Player::render(Spritesheet& spritesheet) {
	blue.render(spritesheet);
	pink.render(spritesheet);
}

void Player::render(Spritesheet& spritesheet, Camera& camera) {
	blue.render(spritesheet, camera);
	pink.render(spritesheet, camera);
}

//void Player::set_spawns(int blue_x, int blue_y, int pink_x, int pink_y) {
//	blue = Square(0, blue_x, blue_y);
//	pink = Square(4, pink_x, pink_y);
//}

void Player::reset_stats() {
	death_count = 0;
	orb_count = 0;
}

void Player::reset_player_positions(LevelHandler& level_handler) {
	// Reset blue
	blue.set_x(level_handler.level_spawn_blue_x);
	blue.set_y(level_handler.level_spawn_blue_y);

	// Reset pink
	pink.set_x(level_handler.level_spawn_pink_x);
	pink.set_y(level_handler.level_spawn_pink_y);
}

uint8_t Player::get_death_count() {
	return death_count;
}

uint8_t Player::get_orb_count() {
	return orb_count;
}

float Player::get_blue_x() {
	return blue.get_x();
}

float Player::get_blue_y() {
	return blue.get_y();
}

float Player::get_pink_x() {
	return pink.get_x();
}

float Player::get_pink_y() {
	return pink.get_y();
}