#include "Entity.hpp"

Entity::Entity() {
	x = y = 0.0f;
	x_vel = y_vel = 0.0f;

	current_frame = 0;
	animation_timer = 0.0f;
}

Entity::Entity(float x, float y, AnimationFrames animation_frames) {
	this->x = x;
	this->y = y;
	x_vel = y_vel = 0.0f;

	this->animation_frames = animation_frames;

	current_frame = 0;
	animation_timer = 0.0f;
}

void Entity::update(float dt) {
	// Increment timer
	animation_timer += dt;

	// Check if we need to increment current_frame
	if (animation_timer >= animation_frames[current_frame].second) {
		animation_timer -= animation_frames[current_frame].second;
		current_frame++;

		// Reset current_frame to 0 if we've reached the end of the animation
		if (current_frame == animation_frames.size()) {
			current_frame = 0;
		}
	}
}

void Entity::render(Spritesheet& spritesheet) {
	spritesheet.sprite_scaled(animation_frames[current_frame].first, x, y);
}