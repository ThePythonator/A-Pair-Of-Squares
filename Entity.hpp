#pragma once

#include "SDL.h"

#include <vector>
#include <utility>

#include "Spritesheet.hpp"

typedef std::pair<uint16_t, float> AnimationFrame;
typedef std::vector<AnimationFrame> AnimationFrames;

class Entity {
public:
	Entity();
	Entity(uint16_t sprite_index, float x, float y);

	void update(float dt);

	void render(Spritesheet& spritesheet);

	float get_x();
	float get_y();

	void set_x(float x);
	void set_y(float y);

protected:
	float x, y;
	float x_vel, y_vel;

	uint16_t sprite_index;
};

class AnimationHandler {
public:
	AnimationHandler();
	AnimationHandler(AnimationFrames animation_frames);

	void update(float dt);

	void reset();

	uint16_t get_sprite_index();

protected:
	AnimationFrames animation_frames;

	uint16_t current_frame;
	float animation_timer;
};