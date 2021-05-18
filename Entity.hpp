#include "SDL.h"

class Entity {
public:
	Entity();
	Entity(float x, float y);

	void update();

	void render(SDL_Surface* screen);

protected:
	float x, y;
	float x_vel, y_vel;
};