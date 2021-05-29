#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include <stdio.h>
#include <string>
#include <array>

#include "Player.hpp"
#include "Spritesheet.hpp"
#include "InputManager.hpp"
#include "ParticleManager.hpp"
#include "TimerManager.hpp"
#include "Bezier.hpp"
#include "TextManager.hpp"

#include "Assets.hpp"

bool init();
void quit();

void load_data();
void clear_data();

int main(int argc, char* argv[]);

void update(float dt);
void render();

// Update and render functions for each state
void update_menu_intro(float dt);
void render_menu_intro();

void update_menu_title(float dt);
void render_menu_title();

SDL_Texture* load_texture(std::string path);

enum class GameState {
	MENU_INTRO,
	MENU_TITLE,
	MENU_SOMETHING_ELSE,
	GAME_SOMETHING
};


// Timer IDs (are set later in program)
namespace TIMER_ID {
	const uint8_t UNINITIALISED = 255;

	uint8_t INTRO_FADE = UNINITIALISED;
	uint8_t BLAH = UNINITIALISED;
}

// Constants

const uint16_t WINDOW_WIDTH = 960;
const uint16_t WINDOW_HEIGHT = 640; //540

const uint8_t SPRITE_SCALE = 4;

const uint8_t SPRITE_SIZE = 16;
const uint8_t SPRITE_SIZE_HALF = SPRITE_SIZE / 2;

const uint16_t SCALED_WINDOW_WIDTH = WINDOW_WIDTH / SPRITE_SCALE;
const uint16_t SCALED_WINDOW_HEIGHT = WINDOW_HEIGHT / SPRITE_SCALE;

const uint16_t SCALED_WINDOW_WIDTH_HALF = SCALED_WINDOW_WIDTH / 2;
const uint16_t SCALED_WINDOW_HEIGHT_HALF = SCALED_WINDOW_HEIGHT / 2;

const char* APP_TITLE = "A Pair of Squares";

const uint8_t FPS = 120;
//const float MIN_DT = 1.0f / FPS;

// Nodes for bezier transitions
namespace BEZIER {
	constexpr std::array<Node, 3> FROM_LEFT(const float y) {
		return { Node{SCALED_WINDOW_WIDTH_HALF, y}, Node{SCALED_WINDOW_WIDTH, y}, Node{-SCALED_WINDOW_WIDTH, y} };
	}

	constexpr std::array<Node, 3> FROM_RIGHT(const float y) {
		return { Node{SCALED_WINDOW_WIDTH_HALF, y}, Node{0, y}, Node{SCALED_WINDOW_WIDTH * 2, y} };
	}
}

namespace DELAY {
	const float TRANSITION_FADE_LENGTH = 0.6f;

	const float INTRO_FADE_START = 2.0f;
	const float INTRO_FADE_LENGTH = 1.0f;
	const float INTRO_LENGTH = INTRO_FADE_START + INTRO_FADE_LENGTH;
}