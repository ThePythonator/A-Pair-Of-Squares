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


// Game class

class Game {
public:
	Game();

	uint8_t run();

private:
	bool init();
	void quit();

	void load_data();
	void clear_data();

	std::string find_assets_path(std::string test_file, uint8_t depth = 4);

	void update(float dt);
	void render();

	// Update and render functions for each state
	void update_menu_intro(float dt);
	void render_menu_intro();

	void update_menu_title(float dt);
	void render_menu_title();


	// Loading functions
	SDL_Texture* load_texture(std::string path);
	SDL_Surface* load_surface(std::string path);
	//SDL_Surface* convert_surface(SDL_Surface* surface);


	enum class GameState {
		MENU_INTRO,
		MENU_TITLE,
		GAME_SOMETHING
	};


	// Variables

	// Main game window
	SDL_Window* window = NULL;

	// Renderer for window
	SDL_Renderer* renderer = NULL;

	// Spritesheet
	SDL_Texture* spritesheet_texture = NULL;
	Spritesheet spritesheet;

	// Fonts
	SDL_Texture* font_sheet_texture = NULL;
	FontHandler::Font font;

	// Game state
	GameState game_state = GameState::MENU_INTRO;

	// Input handler
	InputHandler input_handler;

	// Particle handler
	ParticleHandler particle_handler;

	// Timer handler
	TimerHandler timer_handler;

	// Player
	Player player;
};