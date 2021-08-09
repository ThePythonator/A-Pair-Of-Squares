#include "Utilities.hpp"

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


void SDL_SetRenderDrawColor(SDL_Renderer* renderer, const Colour& colour) {
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
}

void SDL_SetTextureColorMod(SDL_Texture* texture, const Colour& colour) {
	SDL_SetTextureColorMod(texture, colour.r, colour.g, colour.b);
}


bool is_colliding(Tile& tile, float x, float y, uint8_t sprite_size) {
	return (tile.get_x() + sprite_size > x && tile.get_x() < x + sprite_size && tile.get_y() + sprite_size > y && tile.get_y() < y + sprite_size);
}

bool is_colliding(float tile_x, float tile_y, float x, float y, uint8_t sprite_size) {
	return (tile_x + sprite_size > x && tile_x < x + sprite_size && tile_y + sprite_size > y && tile_y < y + sprite_size);
}

bool is_on_tile(Tile& tile, float x, float y, uint8_t sprite_size) {
	return (tile.get_x() + sprite_size > x && tile.get_x() < x + sprite_size && tile.get_y() == y + sprite_size);
}