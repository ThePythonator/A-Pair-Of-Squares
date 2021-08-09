#include "Square.hpp"

Square::Square() : Entity() {

}

Square::Square(uint8_t sprite_index, float x, float y) : Entity(sprite_index, x, y) {

}

void Square::update(std::vector<Tile>& tiles, float dt) {
	if (finished) {
		x_vel = y_vel = 0;
	}
	else {
		// Move y
		y += y_vel * dt;

		// Handle y collisions
		for (Tile& tile : tiles) {
			if (is_colliding(tile, x, y, SPRITE_SIZE)) {
				if (y_vel > 0) {
					// Collided from top
					y = tile.get_y() - SPRITE_SIZE;
				}
				else if (y_vel < 0) {
					// Collided from bottom
					y = tile.get_y() + SPRITE_SIZE;
				}
				y_vel = 0;
			}
		}

		// Move x
		x += x_vel * dt;

		// Handle x collisions
		for (Tile& tile : tiles) {
			if (is_colliding(tile, x, y, SPRITE_SIZE)) {
				if (x_vel > 0) {
					// Collided from left
					x = tile.get_x() - SPRITE_SIZE;
				}
				else if (x_vel < 0) {
					// Collided from right
					x = tile.get_x() + SPRITE_SIZE;
				}
				x_vel = 0;
			}
		}
	}
	
	can_jump = false;
	for (Tile& tile : tiles) {
		can_jump = is_on_tile(tile, x, y, SPRITE_SIZE);

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

	if (std::abs(x_vel) < SQUARE_IDLE_VELOCITY_MIN) {
		// Todo: add blinking
		spritesheet.sprite_scaled(index_base, x, y);
	}
	else {
		if (y_vel == 0.0f) {
			index_base += 3;
		}
		else {
			index_base += 2;
		}

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

void Square::set_finished(bool finished) {
	this->finished = finished;
}

bool Square::get_finished() {
	return finished;
}