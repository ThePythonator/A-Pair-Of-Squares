#include "Player.hpp"

Player::Player() {
	// TODO: remove literals ( 0 and 4 )
	blue = Square(TILE_ID::PLAYER::BLUE, 0, 0);
	pink = Square(TILE_ID::PLAYER::PINK, 0, 0);
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
		blue.accelerate(GAME::SQUARE::ACCELERATION, GAME::SQUARE::VELOCITY_MAX, dt);
		pink.accelerate(-GAME::SQUARE::DECELERATION, GAME::SQUARE::VELOCITY_MAX, dt);
	}
	else if (KeyHandler::is_down(input_handler.get_key_union().keys.LEFT)) {
		blue.accelerate(-GAME::SQUARE::ACCELERATION, GAME::SQUARE::VELOCITY_MAX, dt);
		pink.accelerate(GAME::SQUARE::DECELERATION, GAME::SQUARE::VELOCITY_MAX, dt);
	}
	else {
		// Handle deceleration
		blue.decelerate(GAME::SQUARE::DECELERATION, dt);
		pink.decelerate(GAME::SQUARE::DECELERATION, dt);
	}

	if (KeyHandler::is_down(input_handler.get_key_union().keys.SPACE)) {
		blue.jump(GAME::SQUARE::JUMP_STRENGTH);
		pink.jump(GAME::SQUARE::JUMP_STRENGTH);
	}

	// Update player physics
	blue.gravity(GAME::ENVIRONMENT::GRAVITY::ACCELERATION, GAME::ENVIRONMENT::GRAVITY::MAX, dt);
	pink.gravity(GAME::ENVIRONMENT::GRAVITY::ACCELERATION, GAME::ENVIRONMENT::GRAVITY::MAX, dt);

	// Draw players towards finishes if in both are in range
	if (std::abs(blue.get_x() - level_handler.level_finish_blue_x) < level_handler.get_sprite_size() &&
		std::abs(blue.get_y() - (level_handler.level_finish_blue_y - GAME::FINISH::HEIGHT)) < level_handler.get_sprite_size() / 2 &&
		std::abs(pink.get_x() - level_handler.level_finish_pink_x) < level_handler.get_sprite_size() &&
		std::abs(pink.get_y() - (level_handler.level_finish_pink_y - GAME::FINISH::HEIGHT)) < level_handler.get_sprite_size() / 2 &&
		!blue.get_finished() && !pink.get_finished()) {

		//float blue_vel_x = GAME::SQUARE::FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_x - blue.get_x());
		//float blue_vel_y = 0.0f;//SQAURE_FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_y - blue.get_y());

		//float pink_vel_x = GAME::SQUARE::FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_x - pink.get_x());
		//float pink_vel_y = 0.0f;//GAME::SQUARE::FINISH_PULL_MIN_VELOCITY * (level_handler.level_finish_pink_y - pink.get_y());

		/*if (std::abs(blue_vel_x) < GAME::SQUARE::FINISH_PULL_MIN_VELOCITY) {
			blue_vel_x = GAME::SQUARE::FINISH_PULL_MIN_VELOCITY * blue_vel_x > 0 ? 1 : -1;
		}
		if (std::abs(pink_vel_x) < GAME::SQUARE::FINISH_PULL_MIN_VELOCITY) {
			pink_vel_x = GAME::SQUARE::FINISH_PULL_MIN_VELOCITY * pink_vel_x > 0 ? 1 : -1;
		}*/

		//blue.add_velocity(blue_vel_x, blue_vel_y);
		//pink.add_velocity(pink_vel_x, pink_vel_y);

		//blue.add_velocity(GAME::SQUARE::FINISH_PULL_VELOCITY * (level_handler.level_finish_blue_x - blue.get_x() > 0 ? 1 : -1), 0.0f);
		//pink.add_velocity(GAME::SQUARE::FINISH_PULL_VELOCITY * (level_handler.level_finish_pink_x - pink.get_x() > 0 ? 1 : -1), 0.0f);

		float blue_vel_x = GAME::FINISH::PULL_VELOCITY * (level_handler.level_finish_blue_x - blue.get_x());
		float blue_vel_y = GAME::FINISH::PULL_VELOCITY * (level_handler.level_finish_blue_y - GAME::FINISH::HEIGHT - blue.get_y());

		float pink_vel_x = GAME::FINISH::PULL_VELOCITY * (level_handler.level_finish_pink_x - pink.get_x());
		float pink_vel_y = GAME::FINISH::PULL_VELOCITY * (level_handler.level_finish_pink_y - GAME::FINISH::HEIGHT - pink.get_y());

		if (std::abs(blue_vel_x) < GAME::FINISH::PULL_MIN_VELOCITY) {
			blue_vel_x = GAME::FINISH::PULL_MIN_VELOCITY * blue_vel_x > 0 ? 1 : -1;
		}
		if (std::abs(pink_vel_x) < GAME::FINISH::PULL_MIN_VELOCITY) {
			pink_vel_x = GAME::FINISH::PULL_MIN_VELOCITY * pink_vel_x > 0 ? 1 : -1;
		}

		blue.add_velocity(blue_vel_x, blue_vel_y);
		pink.add_velocity(pink_vel_x, pink_vel_y);
	}


	// Launch players if on spring at right time
	for (Spring& spring : level_handler.get_springs()) {
		if (spring.should_launch()) {
			if (spring.check_collision(blue.get_x(), blue.get_y())) {
				// Reset velocity first to hopefully fix 'super-jump' bug
				blue.reset_y_vel();
				blue.add_velocity(0.0f, -GAME::SPRING::LAUNCH_VELOCITY);
			}

			if (spring.check_collision(pink.get_x(), pink.get_y())) {
				pink.reset_y_vel();
				pink.add_velocity(0.0f, -GAME::SPRING::LAUNCH_VELOCITY);
			}
		}
	}

	blue.update(tiles, level_handler.get_springs(), level_handler.get_buttons(), level_handler.get_doors(), dt);
	pink.update(tiles, level_handler.get_springs(), level_handler.get_buttons(), level_handler.get_doors(), dt);

	// Update player position to allow walking over finish
	if (is_colliding(level_handler.level_finish_blue_x + GAME::FINISH::BORDER, level_handler.level_finish_blue_y + SPRITES::SIZE - GAME::FINISH::HEIGHT,
		blue.get_x(), blue.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {

		blue.set_y(level_handler.level_finish_blue_y - GAME::FINISH::HEIGHT);
		blue.reset_y_vel();
	}
	else if (is_colliding(level_handler.level_finish_pink_x + GAME::FINISH::BORDER, level_handler.level_finish_pink_y + SPRITES::SIZE - GAME::FINISH::HEIGHT,
		blue.get_x(), blue.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {

		blue.set_y(level_handler.level_finish_pink_y - GAME::FINISH::HEIGHT);
		blue.reset_y_vel();
	}

	if (is_colliding(level_handler.level_finish_blue_x + GAME::FINISH::BORDER, level_handler.level_finish_blue_y + SPRITES::SIZE - GAME::FINISH::HEIGHT,
		pink.get_x(), pink.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {

		pink.set_y(level_handler.level_finish_blue_y - GAME::FINISH::HEIGHT);
		pink.reset_y_vel();
	}
	else if (is_colliding(level_handler.level_finish_pink_x + GAME::FINISH::BORDER, level_handler.level_finish_pink_y + SPRITES::SIZE - GAME::FINISH::HEIGHT,
		pink.get_x(), pink.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {

		pink.set_y(level_handler.level_finish_pink_y - GAME::FINISH::HEIGHT);
		pink.reset_y_vel();
	}



	// Blue collision with finishes
	if (!blue.get_can_jump()) {
		if (check_on_top(level_handler.level_finish_blue_x + GAME::FINISH::BORDER, level_handler.level_finish_blue_y + SPRITES::SIZE - GAME::FINISH::HEIGHT, blue.get_x(), blue.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {
			blue.set_can_jump();
		}
	}
	if (!blue.get_can_jump()) {
		if (check_on_top(level_handler.level_finish_pink_x + GAME::FINISH::BORDER, level_handler.level_finish_pink_y + SPRITES::SIZE - GAME::FINISH::HEIGHT, blue.get_x(), blue.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {
			blue.set_can_jump();
		}
	}
	// Pink collision with finishes
	if (!pink.get_can_jump()) {
		if (check_on_top(level_handler.level_finish_blue_x + GAME::FINISH::BORDER, level_handler.level_finish_blue_y + SPRITES::SIZE - GAME::FINISH::HEIGHT, pink.get_x(), pink.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {
			pink.set_can_jump();
		}
	}
	if (!pink.get_can_jump()) {
		if (check_on_top(level_handler.level_finish_pink_x + GAME::FINISH::BORDER, level_handler.level_finish_pink_y + SPRITES::SIZE - GAME::FINISH::HEIGHT, pink.get_x(), pink.get_y(), GAME::FINISH::WIDTH, GAME::FINISH::HEIGHT, SPRITES::SIZE, SPRITES::SIZE)) {
			pink.set_can_jump();
		}
	}


	// If players are already on finish, keep them locked
	if (std::abs(blue.get_x() - level_handler.level_finish_blue_x) < GAME::FINISH::MIN_DISTANCE &&
		std::abs(blue.get_y() - (level_handler.level_finish_blue_y - GAME::FINISH::HEIGHT)) < GAME::FINISH::MIN_DISTANCE &&
		std::abs(pink.get_x() - level_handler.level_finish_pink_x) < GAME::FINISH::MIN_DISTANCE &&
		std::abs(pink.get_y() - (level_handler.level_finish_pink_y - GAME::FINISH::HEIGHT)) < GAME::FINISH::MIN_DISTANCE) {

		blue.set_x(level_handler.level_finish_blue_x);
		blue.set_y(level_handler.level_finish_blue_y - GAME::FINISH::HEIGHT);
		pink.set_x(level_handler.level_finish_pink_x);
		pink.set_y(level_handler.level_finish_pink_y - GAME::FINISH::HEIGHT);

		blue.set_finished();
		pink.set_finished();

		pink.reset_x_vel();
		blue.reset_x_vel();
		pink.reset_y_vel();
		blue.reset_y_vel();
	}

	// Handle orb collisions
	orb_count += level_handler.handle_orb_collisions(blue.get_x(), blue.get_y(), 0);
	orb_count += level_handler.handle_orb_collisions(pink.get_x(), pink.get_y(), 1);

	// Handle spike collisions
	if (level_handler.handle_spike_collisions(blue.get_x(), blue.get_y()) ||
		level_handler.handle_spike_collisions(pink.get_x(), pink.get_y())) {
		// Blue or pink hit spikes

		// Start square death animation
		blue.set_dead();
		pink.set_dead();
	}

	// Check if square death animations have completed
	if (blue.get_fade_finished() && pink.get_fade_finished()) {
		// Reset position
		reset_players(level_handler);

		// Add to death count
		death_count++;
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

void Player::reset_stats() {
	death_count = 0;
	orb_count = 0;
}

void Player::reset_players(LevelHandler& level_handler) {
	// Reset blue position
	blue.set_x(level_handler.level_spawn_blue_x);
	blue.set_y(level_handler.level_spawn_blue_y);

	// Reset pink position
	pink.set_x(level_handler.level_spawn_pink_x);
	pink.set_y(level_handler.level_spawn_pink_y);

	// Reset velocities
	blue.reset_x_vel();
	blue.reset_y_vel();
	pink.reset_x_vel();
	pink.reset_y_vel();

	// Reset fade variables
	blue.reset_dead_fade();
	pink.reset_dead_fade();
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