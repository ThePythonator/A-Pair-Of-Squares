#pragma once

#include "SDL.h"

#include <string>

#include "LevelManager.hpp"
#include "Colour.hpp"

void SDL_SetRenderDrawColor(SDL_Renderer* renderer, const Colour& colour);

void SDL_SetTextureColorMod(SDL_Texture* texture, const Colour& colour);


void set_pixel(SDL_Surface* surface, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
void get_pixel(SDL_Surface* surface, int x, int y, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a);

SDL_Texture* convert_surface(SDL_Renderer* renderer, SDL_Surface* surface);


std::string trim_precision(std::string string, uint8_t precision);

/* 
Generates a random float between 0 and 1 inclusive.
Resolution is 0.001f.
*/
float randf();