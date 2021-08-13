#pragma once

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Orb : public Entity {
public:
	Orb();
	Orb(uint8_t sprite_index, uint8_t type, float x, float y);

	//void update(std::vector<Tile>& tiles, float dt);

	void render(Spritesheet& spritesheet);
	//void render(Spritesheet& spritesheet, Camera& camera);

	/*void accelerate(float acceleration, float maximum, float dt);
	void decelerate(float deceleration, float dt);
	void gravity(float gravity, float maximum, float dt);
	void jump(float strength);*/

	//void add_velocity(float x_vel, float y_vel);

	void set_collected();
	bool get_collected();

	uint8_t get_type();

protected:
	bool collected = false;

	uint8_t type = 0;
};