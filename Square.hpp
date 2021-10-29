#pragma once

#include "SDL.h"

#include "Constants.hpp"

#include "Entity.hpp"
#include "Spritesheet.hpp"
#include "InputManager.hpp"
#include "LevelManager.hpp"
#include "Utilities.hpp"
#include "Door.hpp"

#include <vector>

class Square : public Entity {
public:
	Square();
	Square(uint16_t sprite_index, float x, float y);

	// Returns bool whether player should be dead (if so, level_handler needs to kill both players and reset stuff.
	bool update(std::vector<Tile>& tiles, std::vector<Spring>& springs, std::vector<Button>& buttons, std::vector<Door>& doors, float dt);

	void render(Spritesheet& spritesheet);

	void accelerate(float acceleration, float maximum, float dt);
	void decelerate(float deceleration, float dt);
	void gravity(float gravity, float maximum, float dt);
	void jump(float strength);

	void add_velocity(float x_vel, float y_vel);

	void set_finished(bool finished = true);
	bool get_finished();


	void set_dead();
	bool get_dead();
	void reset_dead_fade();

	bool get_fade_finished();

	void reset_x_vel();
	void reset_y_vel();

	bool get_can_jump();
	void set_can_jump();

protected:

	void set_blink_timer();

	bool can_jump = false;
	bool finished = false;

	bool dead = false;

	float fade_age = 0.0f;

	float blink_timer = 0.0f;

	bool can_blink = false;
};