#pragma once

#include "Square.hpp"
#include "InputManager.hpp"
#include "Spritesheet.hpp"

class Player {
public:
	Player();

	void update(InputHandler& input_handler, float dt);

	void render(Spritesheet& spritesheet);


	//void set_spawns(int blue_x, int blue_y, int pink_x, int pink_y);

protected:
	Square blue, pink;
};