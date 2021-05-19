#pragma once

#include "SDL.h"

#include "Entity.hpp"
#include "Spritesheet.hpp"

class Square : public Entity {
public:
	Square();

	void update(float dt);

	void render(Spritesheet& spritesheet);

protected:

};