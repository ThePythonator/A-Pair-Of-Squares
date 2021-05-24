#include "Square.hpp"

Square::Square() : Entity() {

}

Square::Square(uint8_t sprite_index, float x, float y) : Entity(sprite_index, x, y) {

}

void Square::update(float dt) {
	y += y_vel * dt;
	x += x_vel * dt;
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