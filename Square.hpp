#pragma once

#include "SDL.h"

#include "Entity.hpp"
#include "Spritesheet.hpp"
#include "InputManager.hpp"
#include "LevelManager.hpp"
#include "Utilities.hpp"

//const float DEFAULT_DRAG = 0.98f;
const float SQUARE_ACCELERATION = 500.0f;
const float SQUARE_DECELERATION = 400.0f;
const float SQUARE_VELOCITY_MAX = 120.0f;

const float SQUARE_GRAVITY = 600.0f;
const float SQUARE_GRAVITY_MAX = 250.0f;

const float SQUARE_IDLE_VELOCITY_MIN = 40.0f;

const float SQAURE_JUMP_STRENGTH = 210.0f;

const uint8_t SPRITE_SIZE = 16; // should be in Game.cpp

class Square : public Entity {
public:
	Square();
	Square(uint8_t sprite_index, float x, float y);

	void update(std::vector<Tile>& tiles, float dt);

	void render(Spritesheet& spritesheet);

	void accelerate(float acceleration, float maximum, float dt);
	void decelerate(float deceleration, float dt);
	void gravity(float gravity, float maximum, float dt);
	void jump(float strength);

	void set_finished();

protected:
	bool can_jump = false;
	bool finished = false;
};