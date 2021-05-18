#include "SDL.h"

#include "Entity.hpp"

class Square : public Entity {
public:
	Square();

	void update();

	void render(SDL_Surface* screen);

protected:

};