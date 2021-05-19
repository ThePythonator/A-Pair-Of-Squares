#include "Spritesheet.hpp"

Spritesheet::Spritesheet() {
	renderer = NULL;

	spritesheet_texture = NULL;

	w = h = 0;

	rows = columns = 0;

	sprite_size = 8;
}

Spritesheet::Spritesheet(SDL_Renderer* renderer, SDL_Texture* spritesheet_texture, uint8_t sprite_size) {
	this->renderer = renderer;

	this->spritesheet_texture = spritesheet_texture;

	// Get width and height of spritesheet
	SDL_QueryTexture(spritesheet_texture, NULL, NULL, &w, &h);

	this->sprite_size = sprite_size;

	rows = h / sprite_size;
	columns = w / sprite_size;
}

void Spritesheet::sprite(uint8_t index, int x, int y) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size, sprite_size };
	SDL_RenderCopy(renderer, spritesheet_texture, &src_rect, &dst_rect);
}

void Spritesheet::sprite(uint8_t index, int x, int y, float scale) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size * scale, sprite_size * scale };
	SDL_RenderCopy(renderer, spritesheet_texture, &src_rect, &dst_rect);
}

void Spritesheet::rect(SDL_Rect* rect, int x, int y) {

}