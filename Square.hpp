#include "SDL.h"

#include "Entity.hpp"

class Square : public Entity {
public:
	Square();

	void update(float dt);

	void render();

protected:

};