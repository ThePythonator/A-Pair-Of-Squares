#include <cstdint>

#include "Colour.hpp"

Colour::Colour() {

}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Colour::Colour(const Colour& c) {
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
}

Colour::Colour(const Colour& c, uint8_t a) {
	r = c.r;
	g = c.g;
	b = c.b;
	this->a = a;
}