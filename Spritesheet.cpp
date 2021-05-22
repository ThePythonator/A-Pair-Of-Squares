#include "Spritesheet.hpp"

Spritesheet::Spritesheet() {
	renderer = NULL;

	spritesheet_texture = NULL;

	w = h = 0;

	rows = columns = 0;

	sprite_size = 8;

	scale = 1;
}

Spritesheet::Spritesheet(SDL_Renderer* renderer, SDL_Texture* spritesheet_texture, uint8_t sprite_size, uint8_t scale) {
	this->renderer = renderer;

	this->spritesheet_texture = spritesheet_texture;

	// Get width and height of spritesheet
	SDL_QueryTexture(spritesheet_texture, NULL, NULL, &w, &h);

	this->sprite_size = sprite_size;

	rows = h / sprite_size;
	columns = w / sprite_size;

	this->scale = scale;
}

void Spritesheet::sprite(uint8_t index, float x, float y) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size, sprite_size };
	SDL_RenderCopy(renderer, spritesheet_texture, &src_rect, &dst_rect);
}

void Spritesheet::sprite(uint8_t index, float x, float y, float scale) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size * scale, sprite_size * scale };
	SDL_RenderCopy(renderer, spritesheet_texture, &src_rect, &dst_rect);
}

void Spritesheet::sprite(uint8_t index, float x, float y, float angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size, sprite_size };
	SDL_RenderCopyEx(renderer, spritesheet_texture, &src_rect, &dst_rect, angle, center, flip);
}

void Spritesheet::sprite(uint8_t index, float x, float y, float scale, float angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size * scale, sprite_size * scale };
	SDL_RenderCopyEx(renderer, spritesheet_texture, &src_rect, &dst_rect, angle, center, flip);
}

void Spritesheet::sprite_scaled(uint8_t index, int x, int y) {
	// Render sprite at index from texture to screen
	sprite(index, x * scale, y * scale, scale);
}

void Spritesheet::sprite_scaled(uint8_t index, int x, int y, float angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Render sprite at index from texture to screen
	SDL_Point scaled_center{ center->x * scale, center->y * scale };
	sprite(index, x * scale, y * scale, scale, angle, &scaled_center, flip);
}

void Spritesheet::rect(SDL_Rect* rect, int x, int y) {

}