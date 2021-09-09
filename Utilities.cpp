#include "Utilities.hpp"

void SDL_SetRenderDrawColor(SDL_Renderer* renderer, const Colour& colour) {
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
}

void SDL_SetTextureColorMod(SDL_Texture* texture, const Colour& colour) {
	SDL_SetTextureColorMod(texture, colour.r, colour.g, colour.b);
}


void set_pixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	//SDL_LockSurface(surface);
	((uint32_t*)surface->pixels)[y * surface->w + x] = SDL_MapRGBA(surface->format, r, g, b, a);
	//SDL_UnlockSurface(surface);
}

void get_pixel(SDL_Surface* surface, int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a) {
	//SDL_LockSurface(surface);
	SDL_GetRGBA(((uint32_t*)surface->pixels)[y * surface->w + x], surface->format, r, g, b, a);
	//SDL_UnlockSurface(surface);
}



SDL_Texture* convert_surface(SDL_Renderer* renderer, SDL_Surface* surface) {
	// Create texture from surface
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == NULL)
	{
		printf("Unable to convert texture!\nSDL Error: %s\n", SDL_GetError());
	}

	return texture;
}





std::string trim_precision(std::string string, uint8_t precision) {
	// Trim string to precision
	return string.substr(0, string.find('.') + precision + 1);
}


float randf() {
	return (rand() % 1001) / 1000.0f;
}