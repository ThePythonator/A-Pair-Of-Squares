#include "Utilities.hpp"

void SDL_SetRenderDrawColor(SDL_Renderer* renderer, const Colour& colour) {
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
}

void SDL_SetTextureColorMod(SDL_Texture* texture, const Colour& colour) {
	SDL_SetTextureColorMod(texture, colour.r, colour.g, colour.b);
}




std::string trim_precision(std::string string, uint8_t precision) {
	// Trim string to precision
	return string.substr(0, string.find('.') + precision + 1);
}


float randf() {
	return (rand() % 1001) / 1000.0f;
}