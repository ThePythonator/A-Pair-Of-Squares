#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include <stdio.h>
#include <string>

#include "Square.hpp"
#include "Spritesheet.hpp"
#include "InputManager.hpp"

#include "Assets.hpp"

bool init();
void quit();

void load_data();
void clear_data();

int main(int argc, char* argv[]);

void update(float dt);
void render();

SDL_Texture* load_texture(std::string path);