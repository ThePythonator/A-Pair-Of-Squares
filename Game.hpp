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
#include "Utilities.hpp"

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

	void update_menu_settings(float dt);
	void render_menu_settings();

	void update_menu_level_select(float dt);
	void render_menu_level_select();

	// Setup functions for each state
	void setup_menu_intro();
	void setup_menu_title();
	void setup_menu_settings();
	void setup_menu_level_select();

	// Utility functions
	ImageParticle create_menu_shape_particle();
	void fill_menu_shape_particle(uint8_t count);
	void setup_menu_shape_particles();

	// Loading functions
	SDL_Texture* load_texture(std::string path);
	SDL_Surface* load_surface(std::string path);
	//SDL_Surface* convert_surface(SDL_Surface* surface);


	enum class GameState {
		MENU_INTRO,
		MENU_TITLE,
		MENU_SETTINGS,

		MENU_LEVEL_SELECT,

		GAME_RUNNING,
		GAME_PAUSED
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
	FontHandler::Font /*font_black,*/ font_white, font_selected;

	// Game state
	GameState game_state = GameState::MENU_INTRO;

	// Input handler
	InputHandler input_handler;

	// Particle handler
	struct {
		ParticleHandler back, front;
	} particle_handlers;

	// Timer handler
	TimerHandler timer_handler;

	// Player
	Player player;


	// State variables

	bool running = false;

	uint8_t option_selected = 0;
	bool option_confirmed = false;

	uint8_t shape_particle_count = 0;

	struct {
		bool audio_music = true;
		bool audio_sfx = true;
	} settings;
};