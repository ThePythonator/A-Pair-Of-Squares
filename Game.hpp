#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include <stdio.h>
#include <string>
#include <array>


#include "Constants.hpp"

#include "Player.hpp"
#include "Spritesheet.hpp"
#include "InputManager.hpp"
#include "ParticleManager.hpp"
#include "TimerManager.hpp"
#include "Bezier.hpp"
#include "TextManager.hpp"
//#include "Utilities.hpp"
#include "LevelManager.hpp"
#include "TransitionManager.hpp"
#include "AudioManager.hpp"

#include "JSONManager.hpp"

#include "Assets.hpp"


// Game class

class Game {
public:
	Game();

	void run();

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
	void setup_game_running(uint8_t level_number);
	void setup_game_paused();
	void setup_game_end();

	void resume_game_running();
	void restart_game();

	// Transition functions
	void render_fade_in_rect(float delay);
	void render_fade_out_rect(float delay);
	
	void render_fade_rect(uint8_t alpha);

	std::pair<float, float> get_bezier_text_positions();

	// Utility functions
	void handle_menu_shape_particles();
	ImageParticle create_menu_shape_particle();

	ImageParticle create_game_finish_particle(float x, float y, uint8_t id);

	void render_hint();

	void fill_menu_shape_particle(uint8_t count);
	void setup_menu_shape_particles();

	bool level_is_completed();

	void handle_music();

	void load_save_data(std::string assets_path);
	void write_save_data(std::string assets_path);



	// Loading functions
	SDL_Texture* load_texture(std::string path, bool display_errors = true);
	SDL_Surface* load_surface(std::string path, bool display_errors = true);


	enum class GameState {
		MENU_INTRO,
		MENU_TITLE,
		MENU_SETTINGS,

		MENU_LEVEL_SELECT,

		GAME_RUNNING,
		GAME_PAUSED,
		GAME_END
	};

	enum class FadeState {
		FADE,
		UNFADE,

		NONE
	};


	// Variables

	// Main game window
	SDL_Window* window = NULL;

	// Renderer for window
	SDL_Renderer* renderer = NULL;

	// Audio
	AudioHandler audio_handler;

	// Spritesheet
	SDL_Texture* spritesheet_texture = NULL;
	Spritesheet spritesheet;

	// Icon
	SDL_Surface* window_icon_surface = NULL;

	// Fonts
	SDL_Texture* font_sheet_texture = NULL;
	FontHandler::Font font_white, font_selected, font_highlighted, font_hint;// , font_title_blue, font_title_pink;

	// Input handler
	InputHandler input_handler;

	// Particle handler
	struct {
		ParticleHandler back, front, spare;
	} particle_handlers;

	// Timer handler
	TimerHandler timer_handler;

	// Transition handlers
	TransitionHandler pause_transition, resetting_transition;//menu_transition, 

	// Player
	Player player;

	// Level handler
	LevelHandler level_handler;


	// State/data variables

	// Game state
	GameState game_state = GameState::MENU_INTRO;

	// Fade state
	FadeState fade_state = FadeState::NONE;

	bool running = false;

	bool paused = false;

	bool resetting = false;

	bool intro_music_started = false;

	bool show_level_locked_message = false;

	uint8_t option_selected = 0;
	bool option_confirmed = false;

	uint8_t current_level = 0;

	uint8_t menu_shape_particle_count = 0;

	// Todo: load/save from file
	struct {
		bool audio_music = true;
		bool audio_sfx = true;
		//bool show_hints = true;

		uint8_t level_reached = 0;
		
		std::vector<float> highscore_times;
		std::vector<uint8_t> highscore_orbs;
	} data;

	struct {
		bool time = false;
		bool orbs = false;
	} new_highscore;

	std::string assets_path;
};

// Transition functions
// Not sure why can't be inside Game scope
void pause_transition_update(TransitionState* transition_state, float* timer, float dt);
void pause_transition_render(TransitionState* transition_state, float* timer, SDL_Renderer* renderer, Spritesheet& spritesheet);

void resetting_transition_update(TransitionState* transition_state, float* timer, float dt);
void resetting_transition_render(TransitionState* transition_state, float* timer, SDL_Renderer* renderer, Spritesheet& spritesheet);