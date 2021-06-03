#include "Spritesheet.hpp"

Spritesheet::Spritesheet() {
	w = h = pitch = 0;
	rows = columns = 0;

	sprite_size = 8;
	scale = 1;
}

Spritesheet::Spritesheet(SDL_Renderer* renderer, SDL_Texture* spritesheet_texture, uint8_t sprite_size, uint8_t scale) {
	this->renderer = renderer;
	this->spritesheet_texture = spritesheet_texture;

	// Get width and height of spritesheet
	SDL_QueryTexture(spritesheet_texture, NULL, NULL, &w, &h);
	pitch = 0;

	rows = h / sprite_size;
	columns = w / sprite_size;

	this->sprite_size = sprite_size;
	this->scale = scale;
}

void Spritesheet::sprite(uint16_t index, float x, float y) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size, sprite_size };

	SDL_RenderCopy(renderer, spritesheet_texture, &src_rect, &dst_rect);
}

void Spritesheet::sprite(uint16_t index, float x, float y, float scale) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x * scale, y * scale, sprite_size * scale, sprite_size * scale };

	SDL_RenderCopy(renderer, spritesheet_texture, &src_rect, &dst_rect);
}

void Spritesheet::sprite(uint16_t index, float x, float y, float angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x, y, sprite_size, sprite_size };

	if (center != NULL) {
		center->x *= scale;
		center->y *= scale;
	}

	SDL_RenderCopyEx(renderer, spritesheet_texture, &src_rect, &dst_rect, angle, center, flip);
}

void Spritesheet::sprite(uint16_t index, float x, float y, float scale, float angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Render sprite at index from texture to screen
	SDL_Rect src_rect{ sprite_size * (index % columns), sprite_size * (index / columns), sprite_size, sprite_size };
	SDL_Rect dst_rect{ x * scale, y * scale, sprite_size * scale, sprite_size * scale };

	if (center != NULL) {
		center->x *= scale;
		center->y *= scale;
	}

	SDL_RenderCopyEx(renderer, spritesheet_texture, &src_rect, &dst_rect, angle, center, flip);
}

void Spritesheet::sprite_scaled(uint16_t index, float x, float y) {
	// Render sprite at index from texture to screen
	sprite(index, x, y, scale);
}

void Spritesheet::sprite_scaled(uint16_t index, float x, float y, float angle, SDL_Point* center, SDL_RendererFlip flip) {
	// Render sprite at index from texture to screen
	sprite(index, x, y, scale, angle, center, flip);
}

void Spritesheet::rect(SDL_Rect* src_rect, float x, float y) {
	SDL_Rect dst_rect{ x, y, src_rect->w, src_rect->h };

	SDL_RenderCopy(renderer, spritesheet_texture, src_rect, &dst_rect);
}

void Spritesheet::rect(SDL_Rect* src_rect, float x, float y, float scale) {
	SDL_Rect dst_rect{ x * scale, y * scale, src_rect->w * scale, src_rect->h * scale };

	SDL_RenderCopy(renderer, spritesheet_texture, src_rect, &dst_rect);
}

void Spritesheet::rect_scaled(SDL_Rect* src_rect, float x, float y) {
	rect(src_rect, x, y, scale);
}

SDL_Texture* Spritesheet::get_texture() {
	return spritesheet_texture;
}

//SDL_Colour get_pixel(uint16_t x, uint16_t y) {
//
//}