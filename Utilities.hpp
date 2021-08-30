#pragma once

#include "SDL.h"

#include <string>

#include "LevelManager.hpp"
#include "Colour.hpp"

void SDL_SetRenderDrawColor(SDL_Renderer* renderer, const Colour& colour);

void SDL_SetTextureColorMod(SDL_Texture* texture, const Colour& colour);


std::string trim_precision(std::string string, uint8_t precision);

/* 
Generates a random float between 0 and 1 inclusive.
Resolution is 0.001f.
*/
float randf();