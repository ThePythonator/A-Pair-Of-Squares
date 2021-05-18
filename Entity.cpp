#include "Entity.hpp"

Entity::Entity() {
	x = y = 0.0f;
	x_vel = y_vel = 0.0f;
}

Entity::Entity(float x, float y) {
	this->x = x;
	this->y = y;
	x_vel = y_vel = 0.0f;
}