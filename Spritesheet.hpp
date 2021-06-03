#pragma once

#include "SDL.h"

class Spritesheet {
public:
	Spritesheet();
	Spritesheet(SDL_Renderer* renderer, SDL_Texture* spritesheet_texture, uint8_t sprite_size = 16, uint8_t scale = 1);

	/*void sprite(uint8_t index, int x, int y);
	void sprite(uint8_t index, int x, int y, float scale);*/

	void sprite(uint16_t index, float x, float y);
	void sprite(uint16_t index, float x, float y, float scale);

	void sprite(uint16_t index, float x, float y, float angle, SDL_Point* center, SDL_RendererFlip flip);
	void sprite(uint16_t index, float x, float y, float scale, float angle, SDL_Point* center, SDL_RendererFlip flip);

	void sprite_scaled(uint16_t index, float x, float y);

	void sprite_scaled(uint16_t index, float x, float y, float angle, SDL_Point* center, SDL_RendererFlip flip);

	void rect(SDL_Rect* src_rect, float x, float y);
	void rect(SDL_Rect* src_rect, float x, float y, float scale);

	void rect_scaled(SDL_Rect* src_rect, float x, float y);

	SDL_Texture* get_texture();

	//SDL_Colour get_pixel(uint16_t x, uint16_t y);

private:
	SDL_Renderer* renderer = NULL;

	SDL_Texture* spritesheet_texture = NULL;

	void* pixels = NULL;

	int w, h, pitch;
	uint8_t rows, columns;

	uint8_t sprite_size;

	uint8_t scale;
};