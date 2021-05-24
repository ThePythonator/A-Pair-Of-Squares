#pragma once

#include "SDL.h"

#include "Entity.hpp"
#include "Spritesheet.hpp"
#include "InputManager.hpp"

//const float DEFAULT_DRAG = 0.98f;
const float SQUARE_ACCELERATION = 400.0f;
const float SQUARE_DECELERATION = 300.0f;
const float SQUARE_VELOCITY_MAX = 100.0f;

const float SQUARE_GRAVITY = 600.0f;
const float SQUARE_GRAVITY_MAX = 250.0f;

const float SQUARE_IDLE_VELOCITY_MIN = 40.0f;

class Square : public Entity {
public:
	Square();
	Square(uint8_t sprite_index, float x, float y);

	void update(float dt);

	void render(Spritesheet& spritesheet);

	void accelerate(float acceleration, float maximum, float dt);
	void decelerate(float deceleration, float dt);
	void gravity(float gravity, float maximum, float dt);

protected:

};