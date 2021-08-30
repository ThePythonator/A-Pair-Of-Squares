#include "Square.hpp"

Square::Square() : Entity() {

}

Square::Square(uint16_t sprite_index, float x, float y) : Entity(sprite_index, x, y) {

}

void Square::update(std::vector<Tile>& tiles, float dt) {
	if (can_blink) {
		blink_timer -= dt;
		if (blink_timer <= 0.0f) {
			set_blink_timer();
		}
	}

	if (dead) {
		fade_age += dt;
	}

	if (finished) {
		x_vel = y_vel = 0;
	}
	else {
		// Move y
		y += y_vel * dt;

		// Handle y collisions
		for (Tile& tile : tiles) {
			if (is_colliding(tile, x, y, SPRITES::SIZE)) {
				if (y_vel > 0.0f) {
					// Collided from top
					y = tile.get_y() - SPRITES::SIZE;
				}
				else if (y_vel < 0.0f) {
					// Collided from bottom
					y = tile.get_y() + SPRITES::SIZE;
				}

				y_vel = 0.0f;
			}
		}

		// Move x
		x += x_vel * dt;

		// Handle x collisions
		for (Tile& tile : tiles) {
			if (is_colliding(tile, x, y, SPRITES::SIZE)) {
				if (x_vel > 0) {
					// Collided from left
					x = tile.get_x() - SPRITES::SIZE;
				}
				else if (x_vel < 0) {
					// Collided from right
					x = tile.get_x() + SPRITES::SIZE;
				}
				x_vel = 0;
			}
		}
	}
	
	can_jump = false;
	for (Tile& tile : tiles) {
		can_jump = is_on_tile(tile, x, y, SPRITES::SIZE);

		if (can_jump) {
			// Don't allow can_jump to be set to false after it's been set to true
			break;
		}
	}
}

void Square::render(Spritesheet& spritesheet) {
	// if AnimatedEntity is created, this will be handled by it for us (but that's a TODO)
	uint8_t index_base = sprite_index;

	if (y_vel < 0.0f) {
		index_base += 16;
	}
	else if (y_vel > 0.0f) {
		index_base += 17;
	}

	if (std::abs(x_vel) > GAME::SQUARE::IDLE_VELOCITY_MIN) {
		if (y_vel == 0.0f) {
			index_base += 3;
		}
		else {
			index_base += 2;
		}
	}

	if (index_base == sprite_index) {
		// Check if we need to blink
		if (can_blink) {
			if (blink_timer <= GAME::SQUARE::BLINK::DURATION) {
				if (blink_timer <= GAME::SQUARE::BLINK::FRAME_DURATIONS[1]) {
					// 2nd frame of blink
					index_base += 2;
				}
				else {
					// 1st frame of blink
					index_base += 1;
				}
			}
			else {
				// Not blinking
			}
		}
		else {
			can_blink = true;
		}
	}
	else {
		// Reset blink timer so that when player is next on stationary image it gets updated
		blink_timer = 0.0f;
		can_blink = false;
	}

	if (dead && fade_age < GAME::SQUARE::FADE::MAX_AGE) {
		// Calculate age ratio
		float age_ratio = (fade_age / GAME::SQUARE::FADE::MAX_AGE);

		// Get alpha from age
		uint8_t alpha = (1.0f - age_ratio) * 0xFF;

		// Get scale from age
		float scale = age_ratio * GAME::SQUARE::FADE::MAX_SCALE_BOOST + spritesheet.get_scale();

		// Calculate scale ratio
		float scale_ratio = spritesheet.get_scale() / scale;

		// Get offset to keep sprites centered
		float offset = SPRITES::SIZE * (scale - spritesheet.get_scale()) / 4.0f;
		//float offset = spritesheet.get_sprite_size() * (scale - spritesheet.get_scale()) / 4.0f;
		//float offset = (spritesheet.get_sprite_size() / 2.0f) * (scale - spritesheet.get_scale()) / 2.0f; // Same as line above but less optimized.

		uint8_t old_alpha = spritesheet.get_alpha();
		spritesheet.set_alpha(alpha);
		spritesheet.sprite(sprite_index, (x - offset) * scale_ratio, (y - offset) * scale_ratio, scale, 0.0f, NULL, x_vel < 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
		spritesheet.set_alpha(old_alpha);
	}
	else {
		spritesheet.sprite_scaled(index_base, x, y, 0.0, NULL, x_vel < 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
	}
}

void Square::accelerate(float acceleration, float maximum, float dt) {
	// Handle acceleration
	x_vel += acceleration * dt;

	if ((acceleration < 0.0f && x_vel > 0.0f) || (acceleration > 0.0f && x_vel < 0.0f)) {
		// Make it easier to change direction
		x_vel += acceleration * dt;
	}

	x_vel = std::min(std::max(x_vel, -maximum), maximum);
}

void Square::decelerate(float deceleration, float dt) {
	// Handle deceleration
	if (x_vel < 0.0f) {
		x_vel += deceleration * dt;
		x_vel = std::min(x_vel, 0.0f);
	}
	else if (x_vel > 0.0f) {
		x_vel -= deceleration * dt;
		x_vel = std::max(x_vel, 0.0f);
	}
}

void Square::gravity(float gravity, float maximum, float dt) {
	// Update gravity
	y_vel += gravity * dt;
	y_vel = std::min(y_vel, maximum);
}

void Square::jump(float strength) {
	if (can_jump) {
		y_vel = -strength;
		// cooldown?
	}
}

void Square::add_velocity(float x_vel, float y_vel) {
	this->x_vel += x_vel;
	this->y_vel += y_vel;
}

//void Square::add_velocity(float x_vel, float y_vel, float dt) {
//	this->x_vel += x_vel * dt;
//	this->y_vel += y_vel * dt;
//}

void Square::set_finished(bool finished) {
	this->finished = finished;
}

bool Square::get_finished() {
	return finished;
}

void Square::set_dead() {
	dead = true;
}

void Square::reset_dead_fade() {
	dead = false;
	fade_age = 0.0f;
}

bool Square::get_fade_finished() {
	return dead && fade_age >= GAME::SQUARE::FADE::MAX_AGE;
}

void Square::set_blink_timer() {
	blink_timer = GAME::SQUARE::BLINK::DURATION + GAME::SQUARE::BLINK::DELAY_MIN + (GAME::SQUARE::BLINK::DELAY_MAX - GAME::SQUARE::BLINK::DELAY_MIN) * randf();
}