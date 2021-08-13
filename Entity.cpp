#include "Entity.hpp"

Entity::Entity() {
	sprite_index = 0;

	x = y = 0.0f;
	x_vel = y_vel = 0.0f;
}

Entity::Entity(uint8_t sprite_index, float x, float y) {
	this->sprite_index = sprite_index;

	this->x = x;
	this->y = y;
	x_vel = y_vel = 0.0f;
}

void Entity::update(float dt) {
	
}

void Entity::render(Spritesheet& spritesheet) {
	spritesheet.sprite_scaled(sprite_index, x, y);
}

float Entity::get_x() {
	return x;
}

float Entity::get_y() {
	return y;
}

void Entity::set_x(float x) {
	this->x = x;
}

void Entity::set_y(float y) {
	this->y = y;
}


// AnimatedEntity?
// has Frame::IDLE, WALK, FALL, JUMP etc? then render() picks best?



AnimationHandler::AnimationHandler() {
	current_frame = 0;
	animation_timer = 0.0f;
}

AnimationHandler::AnimationHandler(AnimationFrames animation_frames) {
	this->animation_frames = animation_frames;

	current_frame = 0;
	animation_timer = 0.0f;
}

void AnimationHandler::update(float dt) {
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

uint8_t AnimationHandler::get_sprite_index() {
	return animation_frames[current_frame].first;
}