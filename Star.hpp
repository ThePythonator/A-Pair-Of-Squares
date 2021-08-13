#pragma once

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Star : public Entity {
public:
	Star();
	Star(uint8_t sprite_index, float x, float y);

	//void update(std::vector<Tile>& tiles, float dt);

	void render(Spritesheet& spritesheet);
	//void render(Spritesheet& spritesheet, Camera& camera);

	/*void accelerate(float acceleration, float maximum, float dt);
	void decelerate(float deceleration, float dt);
	void gravity(float gravity, float maximum, float dt);
	void jump(float strength);*/

	//void add_velocity(float x_vel, float y_vel);

	void set_collected();

protected:
	bool collected = false;
};