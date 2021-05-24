#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include <stdio.h>
#include <string>

#include "Player.hpp"
#include "Spritesheet.hpp"
#include "InputManager.hpp"
#include "ParticleManager.hpp"

#include "Assets.hpp"

bool init();
void quit();

void load_data();
void clear_data();

int main(int argc, char* argv[]);

void update(float dt);
void render();

// Update and render functions for each state
void update_menu_title(float dt);
void render_menu_title();

SDL_Texture* load_texture(std::string path);

enum class GameState {
	MENU_TITLE,
	MENU_SOMETHING_ELSE,
	GAME_SOMETHING
};

// Constants

const uint16_t WINDOW_WIDTH = 960;
const uint16_t WINDOW_HEIGHT = 640; //540

const char* APP_TITLE = "A Pair of Squares";

const uint8_t FPS = 120;
//const float MIN_DT = 1.0f / FPS;

const float DELAY_MENU_SHAPE_SPAWN = 2.0f;

//TODO: create Timers Struct/Class??? - keeps all together and updates all at same time?