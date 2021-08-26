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
//#include "Utilities.hpp"
#include "LevelManager.hpp"

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

	void update_game_running(float dt);
	void render_game_running();

	void update_game_paused(float dt);
	void render_game_paused();

	void update_game_end(float dt);
	void render_game_end();

	// Setup functions for each state
	void setup_menu_intro();
	void setup_menu_title();
	void setup_menu_settings();
	void setup_menu_level_select();
	void setup_game_running();
	void setup_game_paused();
	void setup_game_end();

	// Transition functions
	void render_fade_in_rect(float delay);
	void render_fade_out_rect(float delay);

	std::pair<float, float> get_bezier_text_positions();

	// Utility functions
	void handle_menu_shape_particles();
	ImageParticle create_menu_shape_particle();

	ImageParticle create_game_finish_particle(float x, float y, uint8_t id);

	void fill_menu_shape_particle(uint8_t count);
	void setup_menu_shape_particles();

	bool level_is_completed();


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
		GAME_PAUSED,
		GAME_END
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
		ParticleHandler back, front, spare;
	} particle_handlers;

	// Timer handler
	TimerHandler timer_handler;

	// Player
	Player player;

	// Level handler
	LevelHandler level_handler;


	// State/data variables

	bool running = false;

	uint8_t option_selected = 0;
	bool option_confirmed = false;

	uint8_t menu_shape_particle_count = 0;

	struct {
		bool audio_music = true;
		bool audio_sfx = true;
	} settings;
};