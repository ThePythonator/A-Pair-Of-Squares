#pragma once

#include "SDL.h"
#include <stdio.h>
#include <string>

#include "Square.hpp"

bool init();
void quit();

int main(int argc, char* argv[]);

void update(float dt);
void render();

SDL_Surface* load_image(std::string path);