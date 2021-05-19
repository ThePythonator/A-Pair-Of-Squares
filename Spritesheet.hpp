#pragma once

#include "SDL.h"

class Spritesheet {
public:
	Spritesheet();
	Spritesheet(SDL_Renderer* renderer, SDL_Texture* spritesheet_texture, uint8_t sprite_size = 16, uint8_t scale = 1);

	void sprite(uint8_t index, int x, int y);
	void sprite(uint8_t index, int x, int y, float scale);

	void sprite_scaled(uint8_t index, int x, int y);

	void rect(SDL_Rect* rect, int x, int y);

private:
	SDL_Renderer* renderer;

	SDL_Texture* spritesheet_texture;

	int w, h;
	uint8_t rows, columns;

	uint8_t sprite_size;

	uint8_t scale;
};