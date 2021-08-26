#pragma once

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Orb : public Entity {
public:
	Orb();
	Orb(uint8_t sprite_index, uint8_t type, float x, float y);

	void update(float dt);

	void render(Spritesheet& spritesheet);
	//void render(Spritesheet& spritesheet, Camera& camera);

	void set_collected();
	bool get_collected();

	bool get_finished();

	uint8_t get_type();

protected:
	bool collected = false;

	uint8_t type = 0;

	float fade_age = 0.0f;
};