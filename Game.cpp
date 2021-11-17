#include "Game.hpp"

// Game levels
const uint8_t* asset_levels[] = {
	asset_level_0,
	asset_level_1,
	asset_level_2,
	asset_level_3,
	asset_level_4,
	asset_level_5,
	asset_level_6,
	asset_level_7
};

namespace GAME {
	const uint8_t LEVEL_COUNT = 12;
}

namespace MENU {
	enum class TITLE : uint8_t {
		PLAY,
		SETTINGS,
		QUIT,

		OPTION_COUNT
	};

	enum class SETTINGS : uint8_t {
		MUSIC,
		SFX,
		BACK,

		OPTION_COUNT
	};

	enum class LEVEL_SELECT : uint8_t {
		BACK = GAME::LEVEL_COUNT,

		OPTION_COUNT
	};

	enum class LEVEL_PAUSED : uint8_t {
		RESUME,
		RESTART,
		EXIT,

		OPTION_COUNT
	};
}

Game::Game() {

}

// Methods

bool Game::init() {
	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize!\nSDL Error: %s\n", SDL_GetError());
		return false;
	}

	// Initialise SDL_image
	int img_init_flags = IMG_INIT_PNG; // | IMG_INIT_JPG
	if ((IMG_Init(img_init_flags) & img_init_flags) != img_init_flags) {
		printf("SDL_IMG could not initialize!\nSDL_IMG Error: %s\n", IMG_GetError());
		return false;
	}

	// Initialise audio
	audio_handler.init(8);

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	// Create window
	window = SDL_CreateWindow(
		STRINGS::APP_TITLE.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW::WIDTH,
		WINDOW::HEIGHT,
		SDL_WINDOW_SHOWN
	);

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Set renderer colour and mode
	SDL_SetRenderDrawColor(renderer, COLOURS::BLACK);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return true;
}

void Game::quit() {
	// Save game
	write_save_data(assets_path);

	// Destroy assets
	clear_data();

	// Destroy renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	// Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit audio
	audio_handler.quit();

	// Quit SDL
	IMG_Quit();
	SDL_Quit();
}

void Game::load_data() {
	// Uses spritesheet_texture as a test to check whether we can find the assets folder.
	assets_path = find_assets_path(FILES::SPRITESHEET);

	// Load data such as images
	spritesheet_texture = load_texture(assets_path + FILES::SPRITESHEET);
	font_sheet_texture = load_texture(assets_path + FILES::FONT_SHEET);

	spritesheet = Spritesheet(renderer, spritesheet_texture, SPRITES::SIZE, SPRITES::SCALE);

	SDL_Surface* font_sheet_surface = load_surface(assets_path + FILES::FONT_SHEET);
	SDL_Surface * title_font_blue_sheet_surface = load_surface(assets_path + FILES::TITLE_BLUE_FONT_SHEET);
	SDL_Surface* title_font_pink_sheet_surface = load_surface(assets_path + FILES::TITLE_PINK_FONT_SHEET);

	//font_black = FontHandler::Font(renderer, font_sheet_texture, font_sheet_surface, SPRITES::SIZE, SPRITE_SCALE, COLOURS::BLACK);

	font_white = FontHandler::Font(renderer, font_sheet_surface, SPRITES::SIZE, SPRITES::TEXT_SCALE, COLOURS::WHITE, COLOURS::TRUE_WHITE);
	//font_white = FontHandler::Font(renderer, font_sheet_surface, SPRITES::SIZE, SPRITES::TEXT_SCALE, COLOURS::WHITE);
	font_selected = FontHandler::Font(renderer, font_sheet_surface, SPRITES::SIZE, SPRITES::TEXT_SCALE, COLOURS::SELECTED, COLOURS::TRUE_WHITE);

	font_title_blue = FontHandler::Font(renderer, title_font_blue_sheet_surface, SPRITES::SIZE, SPRITES::TEXT_SCALE, COLOURS::TRUE_WHITE);
	font_title_pink = FontHandler::Font(renderer, title_font_pink_sheet_surface, SPRITES::SIZE, SPRITES::TEXT_SCALE, COLOURS::TRUE_WHITE);

	SDL_FreeSurface(font_sheet_surface);
	SDL_FreeSurface(title_font_blue_sheet_surface);
	SDL_FreeSurface(title_font_pink_sheet_surface);

	// Load audio:
	// Music
	audio_handler.music_samples.push_back(audio_handler.load_music(assets_path + FILES::AUDIO::INTRO));

	for (uint8_t i = 0; i < FILES::AUDIO::GAME_TRACKS.size(); i++) {
		audio_handler.music_samples.push_back(audio_handler.load_music(assets_path + FILES::AUDIO::GAME_TRACKS[i]));
	}

	// SFX
	for (uint8_t i = 0; i < FILES::AUDIO::SFX_SAMPLES.size(); i++) {
		audio_handler.sound_samples.push_back(audio_handler.load_sound(assets_path + FILES::AUDIO::SFX_SAMPLES[i]));
	}

	// Set volumes:
	audio_handler.set_music_volume(AUDIO::MUSIC_VOLUME);
	audio_handler.set_sound_volume(AUDIO::SOUND_VOLUME);

	// Load timers
	TIMER_ID::INTRO_LENGTH = timer_handler.add_timer();

	TIMER_ID::MENU_TRANSITION_FADE = timer_handler.add_timer();
	TIMER_ID::MENU_BEZIER_TEXT = timer_handler.add_timer();
	TIMER_ID::MENU_SHAPE_GENERATION = timer_handler.add_timer();

	TIMER_ID::GAME_FINISH_PARTICLE_GENERATION = timer_handler.add_timer();
	TIMER_ID::GAME_DURATION = timer_handler.add_timer();

	// Load transitions
	//menu_transition = TransitionHandler(renderer);
	pause_transition = TransitionHandler(renderer);

	// Setup transitions
	//menu_transition.set_transition();
	pause_transition.set_transition(pause_transition_update, pause_transition_render);

	// Load level handler
	level_handler = LevelHandler(SPRITES::SIZE);


	// Set Window icon
	window_icon_surface = load_surface(assets_path + FILES::WINDOW_ICON);
	SDL_SetWindowIcon(window, window_icon_surface);

	// Load save data
	load_save_data(assets_path);
	
}

void Game::clear_data() {
	// Free loaded data such as images
	SDL_FreeSurface(window_icon_surface);

	SDL_DestroyTexture(spritesheet_texture);
	spritesheet_texture = NULL;

	SDL_DestroyTexture(font_sheet_texture);
	font_sheet_texture = NULL;

	// Free audio
	audio_handler.free_all();
}

std::string Game::find_assets_path(std::string test_file, uint8_t depth) {
	printf("Attempting to find assets folder...\n");

	std::string base_path = SDL_GetBasePath();
	SDL_Surface* test_surface = load_surface("assets/" + test_file, false);

	if (test_surface != NULL) {
		base_path = "";
	}

	uint8_t count = 0;
	while (test_surface == NULL && count < depth) {
		test_surface = load_surface(base_path + "assets/" + test_file, false);

		if (test_surface == NULL) {
			base_path += "../";
		}

		count++;
	}

	if (test_surface == NULL) {
		return "assets/";
	}

	SDL_FreeSurface(test_surface);

	std::string message = "Found assets folder: " + base_path + "assets/\n\n";

	printf("%s", message.c_str());

	return base_path + "assets/";
}

void Game::run()
{
	// Initialise SDL and globals - if it fails, don't run program
	running = init();

	load_data();

	// Main game loop variables
	float dt = 0.0f;
	uint32_t last_time = SDL_GetTicks();
	uint32_t time = 0;

	SDL_Event sdl_event;

	setup_menu_intro();

	// Main game loop
	while (running) {
		// Update input handler (updates all key states etc)
		input_handler.update();

		// Handle events
		while (SDL_PollEvent(&sdl_event) != 0) {
			if (sdl_event.type == SDL_QUIT) {
				// X (close) is pressed
				running = false;
			}
			else {
				// Delegate to InputHandler
				input_handler.handle_sdl_event(sdl_event);
			}
		}

		// Calculate dt
		time = SDL_GetTicks();
		dt = (time - last_time) / 1000.0f;
		last_time = time;

		// Cap dt - stops game skipping time when window is dragged (which caused collisions to fail)
		dt = std::min(dt, WINDOW::MAX_DT);


		//printf("FPS: %f\n", 1.0f / dt);

		//// Limit framerate - to fix
		//if (dt < 1.0f / WINDOW::MAX_FPS) {
		//	printf("(too high)");
		//	// Wait remaining time
		//	SDL_Delay(1000 * (1.0f / WINDOW::MAX_FPS - dt));
		//	printf(" %f", (1.0f / WINDOW::MAX_FPS - dt));
		//	//SDL_Delay(1000 * (1.0f/120 - dt));

		//	// Calculate dt
		//	time = SDL_GetTicks();
		//	dt = (time - last_time) / 1000.0f;
		//	last_time = time;
		//	printf(" corrected FPS: %f\n", 1.0f / dt);
		//}



		update(dt);

		// Clear the screen
		SDL_SetRenderDrawColor(renderer, COLOURS::BLACK);
		SDL_RenderClear(renderer);

		// Render game
		render();

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Quit everything
	quit();
}

void Game::update(float dt) {
	// Update timers
	timer_handler.update(dt);

	// Update particles
	particle_handlers.back.update(dt);
	particle_handlers.spare.update(dt);
	particle_handlers.front.update(dt);

	switch (game_state) {
	case GameState::MENU_INTRO:
		update_menu_intro(dt);
		break;
	case GameState::MENU_TITLE:
		update_menu_title(dt);
		break;
	case GameState::MENU_SETTINGS:
		update_menu_settings(dt);
		break;
	case GameState::MENU_LEVEL_SELECT:
		update_menu_level_select(dt);
		break;
	case GameState::GAME_RUNNING:
		update_game_running(dt);
		break;
	case GameState::GAME_PAUSED:
		update_game_paused(dt);
		break;
	case GameState::GAME_END:
		update_game_end(dt);
		break;
	default:
		break;
	}

	pause_transition.update(dt);
}

void Game::render() {
	// Render particles
	particle_handlers.back.render(spritesheet);

	switch (game_state) {
	case GameState::MENU_INTRO:
		render_menu_intro();
		break;
	case GameState::MENU_TITLE:
		render_menu_title();
		break;
	case GameState::MENU_SETTINGS:
		render_menu_settings();
		break;
	case GameState::MENU_LEVEL_SELECT:
		render_menu_level_select();
		break;
	case GameState::GAME_RUNNING:
		render_game_running();
		break;
	case GameState::GAME_PAUSED:
		render_game_paused();
		break;
	case GameState::GAME_END:
		render_game_end();
		break;
	default:
		break;
	}

	// Render particles
	particle_handlers.front.render(spritesheet);

	pause_transition.render(spritesheet);
}

// Update and render functions for each state
void Game::update_menu_intro(float dt) {
	if (timer_handler.get_timer_state(TIMER_ID::MENU_TRANSITION_FADE) == TimerState::RUNNING &&
		timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) >= DELAY::MENU_INTRO_FADE_LENGTH) {

		// Time to end intro and switch to title screen
		setup_menu_title();

		// Reset timer ready for use in unfading to title screen
		timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);
		timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);

		fade_state = FadeState::UNFADE;
	}
	else if (timer_handler.get_timer_state(TIMER_ID::INTRO_LENGTH) == TimerState::RUNNING &&
		timer_handler.get_timer(TIMER_ID::INTRO_LENGTH) >= DELAY::MENU_INTRO_LENGTH &&
		!audio_handler.is_music_playing()) {

		if (intro_music_started) {
			// Start fade
			timer_handler.reset_timer(TIMER_ID::INTRO_LENGTH);
			timer_handler.set_timer_state(TIMER_ID::INTRO_LENGTH, TimerState::PAUSED);

			timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);
			timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);

			fade_state = FadeState::FADE;
		}
		else {
			// Play intro music
			audio_handler.play_music(audio_handler.music_samples.at(0), 0);

			intro_music_started = true;
		}
	}
}

void Game::render_menu_intro() {
	// Display logo
	SDL_Rect src_rect{ 0, 224, 32, 32 };
	spritesheet.rect(&src_rect, WINDOW::WIDTH / 16.0f - SPRITES::SIZE, WINDOW::HEIGHT / 16.0f - SPRITES::SIZE, 8);

	// Display fade-out black rect
	if (fade_state == FadeState::FADE) {
		render_fade_out_rect(DELAY::MENU_INTRO_FADE_LENGTH);
	}
}

void Game::update_menu_title(float dt) {
	// Update music (auto-play random piece)
	// TODO: fade in somehow?
	handle_music();

	// Handle creation and removal of shape particles in background
	handle_menu_shape_particles();

	// Stop bezier stuff if finished
	if (timer_handler.get_timer(TIMER_ID::MENU_BEZIER_TEXT) >= DELAY::MENU_BEZIER_LENGTH) {
		timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::PAUSED);
	}

	if (!option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
		// User hasn't selected an option yet

		// Check if up/down has been pressed, and naviagate the menu as necessary
		if (KeyHandler::just_down(input_handler.get_key_union().keys.UP)) {
			if (option_selected > 0) {
				option_selected--;

				// Should we play select sfx?
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.DOWN)) {
			if (option_selected < (uint8_t)MENU::TITLE::OPTION_COUNT - 1) {
				option_selected++;

				// Should we play select sfx?
			}
		}

		// Check if user has just selected an option
		if (KeyHandler::just_down(input_handler.get_key_union().keys.SPACE)) {
			// Reset timer and set it to running
			timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
			timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

			option_confirmed = true;

			// Play 'select' sfx
			audio_handler.play_sound(audio_handler.sound_samples.at(AUDIO::SFX::SELECT));
		}
	}
	else {
		// User has selected an option, or initial intro is still happening

		// Check if transition is finished

		if (option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
			// User has selected an option and animation is finished

			switch ((MENU::TITLE)option_selected)
			{
			case MENU::TITLE::PLAY:
				setup_menu_level_select();
				break;

			case MENU::TITLE::SETTINGS:
				setup_menu_settings();
				break;

			case MENU::TITLE::QUIT:
				running = false;
				break;

			default:
				break;
			}
		}
	}
}

void Game::render_menu_title() {
	// Get x positions of text
	std::pair<float, float> positions = get_bezier_text_positions();

	float left_x = positions.first;
	float right_x = positions.second;


	//TextHandler::render_text(font_title_blue, STRINGS::MENU::TITLE::HEADING_BLUE, right_x, SPRITES::SIZE, SPRITES::SPACE_WIDTH, TextHandler::CENTER_RIGHT);
	//TextHandler::render_text(font_title_pink, STRINGS::MENU::TITLE::HEADING_PINK, right_x + SPRITES::SIZE_HALF, SPRITES::SIZE, SPRITES::SPACE_WIDTH, TextHandler::CENTER_LEFT);
	
	TextHandler::render_text(option_selected == 0 ? font_selected : font_white, STRINGS::MENU::TITLE::OPTION_PLAY, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE * 2, SPRITES::SPACE_WIDTH);
	TextHandler::render_text(option_selected == 1 ? font_selected : font_white, STRINGS::MENU::TITLE::OPTION_SETTINGS, right_x, WINDOW::TEXT_SCALED_HEIGHT_HALF, SPRITES::SPACE_WIDTH);
	TextHandler::render_text(option_selected == 2 ? font_selected : font_white, STRINGS::MENU::TITLE::OPTION_QUIT, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE * 2, SPRITES::SPACE_WIDTH);

	// Display fade-in black rect
	if (fade_state == FadeState::UNFADE) {
		render_fade_in_rect(DELAY::TRANSITION_FADE_LENGTH);
	}
}

void Game::update_menu_settings(float dt) {
	// Update music (auto-play random piece)
	handle_music();

	// Handle creation and removal of shape particles in background
	handle_menu_shape_particles();

	// Stop bezier stuff if finished
	if (timer_handler.get_timer(TIMER_ID::MENU_BEZIER_TEXT) >= DELAY::MENU_BEZIER_LENGTH) {
		timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::PAUSED);
	}

	if (!option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
		// User hasn't selected an option yet

		// Check if up/down has been pressed, and naviagate the menu as necessary
		if (KeyHandler::just_down(input_handler.get_key_union().keys.UP)) {
			if (option_selected > 0) {
				option_selected--;
				// Should we play select sfx?
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.DOWN)) {
			if (option_selected < (uint8_t)MENU::SETTINGS::OPTION_COUNT - 1) {
				option_selected++;
				// Should we play select sfx?
			}
		}

		// Check if user has just selected an option
		if (KeyHandler::just_down(input_handler.get_key_union().keys.SPACE)) {
			// Play 'select' sfx
			audio_handler.play_sound(audio_handler.sound_samples.at(AUDIO::SFX::SELECT));

			// Handle settings/back
			switch ((MENU::SETTINGS)option_selected)
			{
			case MENU::SETTINGS::MUSIC:
				data.audio_music = !data.audio_music;
				break;

			case MENU::SETTINGS::SFX:
				data.audio_sfx = !data.audio_sfx;

				// Turn sfx volume on/off
				audio_handler.set_sound_volume(data.audio_sfx ? AUDIO::SOUND_VOLUME : 0.0f);
				break;

			case MENU::SETTINGS::BACK:
				// We need bezier curves

				// Reset timer and set it to running
				timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
				timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

				option_confirmed = true;
				break;

			default:
				break;
			}
		}
	}
	else {
		// User has selected an option, or initial intro is still happening

		// Check if transition is finished
		if (option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
			// User has selected an option and animation is finished

			if ((MENU::SETTINGS)option_selected == MENU::SETTINGS::BACK) {
				setup_menu_title();
			}
		}
	}
}

void Game::render_menu_settings() {
	// Get x positions of text
	std::pair<float, float> positions = get_bezier_text_positions();

	float left_x = positions.first;
	float right_x = positions.second;


	TextHandler::render_text(option_selected == 0 ? font_selected : font_white, STRINGS::MENU::SETTINGS::OPTION_MUSIC + STRINGS::COLON_SPACE + (data.audio_music ? STRINGS::ON : STRINGS::OFF), left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE * 2, SPRITES::SPACE_WIDTH);
	TextHandler::render_text(option_selected == 1 ? font_selected : font_white, STRINGS::MENU::SETTINGS::OPTION_SFX + STRINGS::COLON_SPACE + (data.audio_sfx ? STRINGS::ON : STRINGS::OFF), right_x, WINDOW::TEXT_SCALED_HEIGHT_HALF, SPRITES::SPACE_WIDTH);
	TextHandler::render_text(option_selected == 2 ? font_selected : font_white, STRINGS::MENU::SETTINGS::OPTION_BACK, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE * 2, SPRITES::SPACE_WIDTH);
}

void Game::update_menu_level_select(float dt) {
	// Update music (auto-play random piece)
	handle_music();

	// Handle creation and removal of shape particles in background
	handle_menu_shape_particles();

	// Stop bezier stuff if finished
	if (timer_handler.get_timer(TIMER_ID::MENU_BEZIER_TEXT) >= DELAY::MENU_BEZIER_LENGTH) {
		timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::PAUSED);
	}

	if (!option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
		// User hasn't selected an option yet

		// Check if up/down has been pressed, and naviagate the menu as necessary
		if (KeyHandler::just_down(input_handler.get_key_union().keys.UP)) {
			show_level_locked_message = false;

			/*if (option_selected == (uint8_t)MENU::LEVEL_SELECT::BACK) {
				option_selected--;
			}
			else */
			if (option_selected > 3) {
				option_selected -= 4;
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.DOWN)) {
			show_level_locked_message = false;

			if (option_selected < (uint8_t)MENU::LEVEL_SELECT::OPTION_COUNT - 5) {
				option_selected += 4;
				// Should we play select sfx?
			}
			else if (option_selected < (uint8_t)MENU::LEVEL_SELECT::OPTION_COUNT - 1) {
				option_selected = (uint8_t)MENU::LEVEL_SELECT::BACK;
				// Should we play select sfx?
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.LEFT)) {
			show_level_locked_message = false;

			if (option_selected % 4 > 0) {
				option_selected--;
				// Should we play select sfx?
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.RIGHT)) {
			show_level_locked_message = false;

			if (option_selected % 4 < 3 && option_selected != (uint8_t)MENU::LEVEL_SELECT::OPTION_COUNT - 1) {
				option_selected++;
				// Should we play select sfx?
			}
		}

		// Check if user has just selected an option
		if (KeyHandler::just_down(input_handler.get_key_union().keys.SPACE)) {
			if (option_selected == (uint8_t)MENU::LEVEL_SELECT::BACK || option_selected <= data.level_reached) {
				show_level_locked_message = false;

				// Reset timer and set it to running
				timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
				timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

				option_confirmed = true;

				// Play 'select' sfx
				audio_handler.play_sound(audio_handler.sound_samples.at(AUDIO::SFX::SELECT));
			}
			else {
				// Level locked!
				show_level_locked_message = true;
			}
		}
	}
	else if (timer_handler.get_timer_state(TIMER_ID::MENU_TRANSITION_FADE) == TimerState::PAUSED) {
		// User has selected an option, or initial intro is still happening

		// NOTE: possible bug - transition fade to in-game seems to not start until bezier has finished!

		// Check if transition is finished
		if (option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
			// User has selected an option and animation is finished

			if ((MENU::LEVEL_SELECT)option_selected == MENU::LEVEL_SELECT::BACK) {
				setup_menu_title();
			}
			else {
				// User has selected a level
				// level number is option_selected

				// Reset timer ready for use in unfading to title screen
				timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);
				timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);

				fade_state = FadeState::FADE;
			}
		}
	}
	else {
		// Fade timer isn't paused, so we're transitioning to game

		if (timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) > DELAY::TRANSITION_FADE_LENGTH) {
			timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::PAUSED);

			setup_game_running(option_selected);
		}
	}
}

void Game::render_menu_level_select() {
	// Get x positions of text
	std::pair<float, float> positions = get_bezier_text_positions();

	float left_x = positions.first;
	float right_x = positions.second;

	for (uint8_t i = 0; i < GAME::LEVEL_COUNT; i++) {
		float x = ((i % MENU::LEVELS_PER_ROW) - MENU::LEVELS_PER_ROW / 2 + 0.5f) * SPRITES::SIZE * 2;
		float y = (i / MENU::LEVELS_PER_ROW) * SPRITES::SIZE * 2 + SPRITES::SIZE * 2; // The division is intentionally truncated before casting to float.

		x += (i / 4) % 2 ? left_x : right_x;


		// Modify alpha if locked
		uint8_t old_alpha = (option_selected == i ? font_selected : font_white).get_alpha();
		if (i > data.level_reached) {
			(option_selected == i ? font_selected : font_white).set_alpha(SPRITES::TEXT_LOCKED_ALPHA);
		}

		TextHandler::render_text(option_selected == i ? font_selected : font_white, std::to_string(i + 1), x, y, SPRITES::SPACE_WIDTH);

		if (i > data.level_reached) {
			(option_selected == i ? font_selected : font_white).set_alpha(old_alpha);
		}
	}

	if (show_level_locked_message) {
		uint8_t old_alpha = font_white.get_alpha();

		font_white.set_alpha(SPRITES::TEXT_LOCKED_ALPHA);

		//TextHandler::render_text(font_white, STRINGS::MENU::LEVEL_SELECT::LEVEL_LOCKED, WINDOW::TEXT_SCALED_WIDTH_HALF, SPRITES::SIZE, SPRITES::SPACE_WIDTH);
		TextHandler::render_text(font_white, STRINGS::MENU::LEVEL_SELECT::LEVEL_LOCKED, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE * 3, SPRITES::SPACE_WIDTH);

		font_white.set_alpha(old_alpha);
	}
	else {
		TextHandler::render_text(option_selected == (uint8_t)MENU::LEVEL_SELECT::BACK ? font_selected : font_white, STRINGS::MENU::LEVEL_SELECT::OPTION_BACK, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE * 3, SPRITES::SPACE_WIDTH);
	}

	// Display fade-out black rect
	if (fade_state == FadeState::FADE) {
		render_fade_out_rect(DELAY::TRANSITION_FADE_LENGTH);
	}
}

void Game::update_game_running(float dt) {
	// Update music (auto-play random piece)
	handle_music();

	// Handle creation and removal of shape particles in background
	handle_menu_shape_particles();

	// Handle particles from finish
	if (timer_handler.get_timer(TIMER_ID::GAME_FINISH_PARTICLE_GENERATION) >= DELAY::GAME_FINISH_PARTICLE_GENERATION) {
		timer_handler.reset_timer(TIMER_ID::GAME_FINISH_PARTICLE_GENERATION);

		particle_handlers.spare.add(create_game_finish_particle(level_handler.level_finish_blue_x, level_handler.level_finish_blue_y + SPRITES::SIZE, 0));
		particle_handlers.spare.add(create_game_finish_particle(level_handler.level_finish_pink_x, level_handler.level_finish_pink_y + SPRITES::SIZE, 1));
	}
	
	particle_handlers.spare.remove_if([](ImageParticle& particle) { return particle.get_alpha() == 0.0f; });

	// Only move the player and entities when there aren't transitions in the way
	if (!paused && fade_state == FadeState::NONE) {
		player.update(input_handler, audio_handler, level_handler, dt);

		level_handler.update(audio_handler, dt);
	}

	if (timer_handler.get_timer(TIMER_ID::GAME_DURATION) == 0.0f && timer_handler.get_timer_state(TIMER_ID::GAME_DURATION) == TimerState::PAUSED) {
		// Level has not started yet
		if (timer_handler.get_timer_state(TIMER_ID::MENU_TRANSITION_FADE) == TimerState::PAUSED) {
			// Transition fade is complete, start game timer
			timer_handler.set_timer_state(TIMER_ID::GAME_DURATION, TimerState::RUNNING);
		}
	}
	else if (level_is_completed()) {
		// Level has ended

		// Update max level number
		if (current_level == data.level_reached && data.level_reached < GAME::LEVEL_COUNT) {
			data.level_reached++;
		}

		if (timer_handler.get_timer_state(TIMER_ID::MENU_TRANSITION_FADE) == TimerState::PAUSED) {
			if (timer_handler.get_timer_state(TIMER_ID::GAME_DURATION) == TimerState::PAUSED) {
				// End of transition
				setup_game_end();
			}
			else {
				// Pause game timer for use on stats screen
				timer_handler.set_timer_state(TIMER_ID::GAME_DURATION, TimerState::PAUSED);

				// Start transition fade
				timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);
				timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);

				fade_state = FadeState::FADE;
			}
		}
	}
	else if (!paused) {
		// Level in progress
		if (KeyHandler::just_down(input_handler.get_key_union().keys.ESCAPE)) {
			// User has paused game
			paused = true;

			// Pause game timer
			timer_handler.set_timer_state(TIMER_ID::GAME_DURATION, TimerState::PAUSED);

			// Start transition fade
			//timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);
			//timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);

			//fade_state = FadeState::FADE;
			pause_transition.close();

			// Should we play select sfx?
		}
	}

	if (pause_transition.is_closed() && paused) {
		setup_game_paused();
	}
	else if (fade_state == FadeState::NONE && !paused) {
		// Unpause game timer
		timer_handler.set_timer_state(TIMER_ID::GAME_DURATION, TimerState::RUNNING);
	}
}

void Game::render_game_running() {
	// Render spare particle handler in front of background particles but behind everything else
	particle_handlers.spare.render(spritesheet);

	level_handler.render(spritesheet);

	player.render(spritesheet);

	// Display fade-in black rect
	if (fade_state == FadeState::UNFADE) {
		render_fade_in_rect(DELAY::TRANSITION_FADE_LENGTH);
	}
	else if (fade_state == FadeState::FADE) {
		render_fade_out_rect(DELAY::TRANSITION_FADE_LENGTH);
	}
}

void Game::update_game_paused(float dt) {
	// Update music (auto-play random piece)
	handle_music();

	// Handle creation and removal of shape particles in background
	handle_menu_shape_particles();

	// Handle particles from finish
	if (timer_handler.get_timer(TIMER_ID::GAME_FINISH_PARTICLE_GENERATION) >= DELAY::GAME_FINISH_PARTICLE_GENERATION) {
		timer_handler.reset_timer(TIMER_ID::GAME_FINISH_PARTICLE_GENERATION);

		particle_handlers.spare.add(create_game_finish_particle(level_handler.level_finish_blue_x, level_handler.level_finish_blue_y + SPRITES::SIZE, 0));
		particle_handlers.spare.add(create_game_finish_particle(level_handler.level_finish_pink_x, level_handler.level_finish_pink_y + SPRITES::SIZE, 1));
	}

	particle_handlers.spare.remove_if([](ImageParticle& particle) { return particle.get_alpha() == 0.0f; });

	// Stop bezier stuff if finished
	if (timer_handler.get_timer(TIMER_ID::MENU_BEZIER_TEXT) >= DELAY::MENU_BEZIER_LENGTH) {
		timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::PAUSED);
	}

	if (!option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
		// User hasn't selected an option yet

		// Check if up/down has been pressed, and naviagate the menu as necessary
		if (KeyHandler::just_down(input_handler.get_key_union().keys.UP)) {
			if (option_selected > 0) {
				option_selected--;
				// Should we play select sfx?
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.DOWN)) {
			if (option_selected < (uint8_t)MENU::LEVEL_PAUSED::OPTION_COUNT - 1) {
				option_selected++;
				// Should we play select sfx?
			}
		}

		// Check if user has just selected an option
		if (KeyHandler::just_down(input_handler.get_key_union().keys.SPACE)) {
			// Handle paused options

			// Play 'select' sfx
			audio_handler.play_sound(audio_handler.sound_samples.at(AUDIO::SFX::SELECT));

			switch ((MENU::LEVEL_PAUSED)option_selected)
			{
			case MENU::LEVEL_PAUSED::RESUME:
				// We need bezier curves
				// Reset timer and set it to running
				timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
				timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

				//timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
				//timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

				//fade_state = FadeState::FADE;

				option_confirmed = true;
				break;


			case MENU::LEVEL_PAUSED::RESTART:
				// We need bezier curves
				// Reset timer and set it to running
				timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
				timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

				//restart_game();

				fade_state = FadeState::FADE;

				option_confirmed = true;
				break;

			case MENU::LEVEL_PAUSED::EXIT:
				// We need bezier curves

				// Reset timer and set it to running
				timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
				timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

				timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
				timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

				fade_state = FadeState::FADE;

				option_confirmed = true;
				break;

			default:
				break;
			}
		}
	}
	else {
		// User has selected an option, or initial intro is still happening

		// Check if transition is finished
		//if (option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
		////if (option_confirmed && timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) > DELAY::TRANSITION_FADE_LENGTH) {
		//	// User has selected an option and animation is finished

		//	if ((MENU::LEVEL_PAUSED)option_selected == MENU::LEVEL_PAUSED::RESUME) {
		//		// Return to game
		//		resume_game_running();
		//	}
		//	else if ((MENU::LEVEL_PAUSED)option_selected == MENU::LEVEL_PAUSED::EXIT) {
		//		// Exit to title
		//		timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
		//		timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

		//		fade_state = FadeState::UNFADE;

		//		setup_menu_title();
		//	}
		//}

		// Check if transition is finished and handle option selected
		if (option_confirmed) {
			switch ((MENU::LEVEL_PAUSED)option_selected) {
			case MENU::LEVEL_PAUSED::RESUME:
				if (timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
					// Return to game
					resume_game_running();
				}
				break;

			case MENU::LEVEL_PAUSED::RESTART:
				//if (timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
				//	// Restart and return to game
				//	resume_game_running();
				//}

				if (timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
					if (timer_handler.get_timer_state(TIMER_ID::MENU_TRANSITION_FADE) == TimerState::PAUSED) {
						timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);
						timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);

						fade_state = FadeState::FADE;
					}
					else if (timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) > DELAY::TRANSITION_FADE_LENGTH) {
						// Restart game
						setup_game_running(current_level);
					}
				}

				break;

			case MENU::LEVEL_PAUSED::EXIT:
				if (timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED && timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) > DELAY::TRANSITION_FADE_LENGTH) {
					// Exit to title
					timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
					timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

					fade_state = FadeState::UNFADE;

					setup_menu_title();
				}
				break;

			default:
				break;
			}
		}
	}
}

void Game::render_game_paused() {
	// Render game scene in background

	// Render spare particle handler in front of background particles but behind everything else
	particle_handlers.spare.render(spritesheet);

	level_handler.render(spritesheet);

	player.render(spritesheet);
	
	// Cause game scene to be partially faded out
	render_fade_rect(MENU::PAUSED_BACKGROUND_ALPHA);

	// Get x positions of text
	std::pair<float, float> positions = get_bezier_text_positions();

	float left_x = positions.first;
	float right_x = positions.second;


	TextHandler::render_text(option_selected == 0 ? font_selected : font_white, STRINGS::MENU::LEVEL_PAUSED::OPTION_RESUME, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE * 2, SPRITES::SPACE_WIDTH);
	//TextHandler::render_text(option_selected == 1 ? font_selected : font_white, STRINGS::MENU::LEVEL_PAUSED::OPTION_SFX + STRINGS::COLON_SPACE + (settings.audio_sfx ? STRINGS::ON : STRINGS::OFF), right_x, WINDOW::TEXT_SCALED_HEIGHT_HALF, SPRITES::SPACE_WIDTH);
	TextHandler::render_text(option_selected == 1 ? font_selected : font_white, STRINGS::MENU::LEVEL_PAUSED::OPTION_RESTART, right_x, WINDOW::TEXT_SCALED_HEIGHT_HALF, SPRITES::SPACE_WIDTH);//left_x
	TextHandler::render_text(option_selected == 2 ? font_selected : font_white, STRINGS::MENU::LEVEL_PAUSED::OPTION_EXIT, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE * 2, SPRITES::SPACE_WIDTH);//left_x
	
	// Display fade-in/out black rect
	if (fade_state == FadeState::UNFADE) {
		render_fade_in_rect(DELAY::TRANSITION_FADE_LENGTH);
		//fade_state = FadeState::UNFADE; //hacky but fixes issue with transition flashing I think
	}
	else if (fade_state == FadeState::FADE) {
		render_fade_out_rect(DELAY::TRANSITION_FADE_LENGTH);
		//fade_state = FadeState::FADE; //hacky but fixes issue with transition flashing I think
	}
	else if (option_confirmed && (MENU::LEVEL_PAUSED)option_selected == MENU::LEVEL_PAUSED::EXIT && timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) > DELAY::TRANSITION_FADE_LENGTH) {
		// Bit hacky but stops flashing when moving to menu when exit is pressed.
		render_fade_rect(0xFF);
	}
}

void Game::update_game_end(float dt) {
	// Update music (auto-play random piece)
	handle_music();

	// Handle creation and removal of shape particles in background
	handle_menu_shape_particles();

	// Stop bezier stuff if finished
	if (timer_handler.get_timer(TIMER_ID::MENU_BEZIER_TEXT) >= DELAY::MENU_BEZIER_LENGTH) {
		timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::PAUSED);
	}

	if (!option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
		// User hasn't selected an option yet

		// Check if user has just selected 'Continue'
		if (KeyHandler::just_down(input_handler.get_key_union().keys.SPACE)) {
			// Start bezier transition
			timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
			timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

			option_confirmed = true;

			// Play 'select' sfx
			audio_handler.play_sound(audio_handler.sound_samples.at(AUDIO::SFX::SELECT));
		}
	}
	else {
		// User has selected an option, or initial intro is still happening

		// Check if transition is finished
		if (option_confirmed && timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::PAUSED) {
			// User has selected an option and animation is finished
			setup_menu_level_select();
		}
	}
}

void Game::render_game_end() {
	// Get x positions of text
	std::pair<float, float> positions = get_bezier_text_positions();

	float left_x = positions.first;
	float right_x = positions.second;


	// Calculate score (has to be signed so that it can be stopped from going below 0)
	int16_t score = GAME::SCORE::INITIAL;
	score += GAME::SCORE::ORB * player.get_orb_count();
	score -= GAME::SCORE::TIME * (uint16_t)timer_handler.get_timer(TIMER_ID::GAME_DURATION);
	score -= GAME::SCORE::DEATH * player.get_death_count();

	if (score < 0) {
		score = 0;
	}

	// Convert to strings
	std::string time_string = trim_precision(std::to_string(timer_handler.get_timer(TIMER_ID::GAME_DURATION)), MENU::FLOAT_TEXT_PRECISION);
	std::string orbs_string = std::to_string(player.get_orb_count());
	std::string death_count_string = std::to_string(player.get_death_count());
	std::string score_string = std::to_string(score);

	// Display stats
	TextHandler::render_text(font_white, STRINGS::MENU::LEVEL_COMPLETED::TEXT_TIME_TAKEN + STRINGS::COLON_SPACE, left_x - SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE_HALF * 7, SPRITES::SPACE_WIDTH, TextHandler::CENTER_LEFT);
	TextHandler::render_text(font_white, STRINGS::MENU::LEVEL_COMPLETED::TEXT_ORBS_COLLECTED + STRINGS::COLON_SPACE, right_x - SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE_HALF * 4, SPRITES::SPACE_WIDTH, TextHandler::CENTER_LEFT);
	TextHandler::render_text(font_white, STRINGS::MENU::LEVEL_COMPLETED::TEXT_NUMBER_OF_DEATHS + STRINGS::COLON_SPACE, left_x - SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE_HALF * 1, SPRITES::SPACE_WIDTH, TextHandler::CENTER_LEFT);
	TextHandler::render_text(font_white, STRINGS::MENU::LEVEL_COMPLETED::TEXT_SCORE + STRINGS::COLON_SPACE, right_x - SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE_HALF * 2, SPRITES::SPACE_WIDTH, TextHandler::CENTER_LEFT);

	TextHandler::render_text(font_white, time_string, left_x + SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE_HALF * 7, SPRITES::SPACE_WIDTH, TextHandler::CENTER_RIGHT);
	TextHandler::render_text(font_white, orbs_string, right_x + SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE_HALF * 4, SPRITES::SPACE_WIDTH, TextHandler::CENTER_RIGHT);
	TextHandler::render_text(font_white, death_count_string, left_x + SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF - SPRITES::SIZE_HALF * 1, SPRITES::SPACE_WIDTH, TextHandler::CENTER_RIGHT);
	TextHandler::render_text(font_white, score_string, right_x + SPRITES::TEXT_OFFSET_X, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE_HALF * 2, SPRITES::SPACE_WIDTH, TextHandler::CENTER_RIGHT);
	
	TextHandler::render_text(font_selected, STRINGS::MENU::LEVEL_COMPLETED::OPTION_CONTINUE, left_x, WINDOW::TEXT_SCALED_HEIGHT_HALF + SPRITES::SIZE_HALF * 7, SPRITES::SPACE_WIDTH);


	// Display fade-in black rect
	if (fade_state == FadeState::UNFADE) {
		render_fade_in_rect(DELAY::TRANSITION_FADE_LENGTH);
	}
}

void Game::setup_menu_intro() {
	game_state = GameState::MENU_INTRO;

	timer_handler.reset_timer(TIMER_ID::INTRO_LENGTH);
	timer_handler.set_timer_state(TIMER_ID::INTRO_LENGTH, TimerState::RUNNING);

	timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);
	timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::PAUSED);
}

void Game::setup_menu_title() {
	game_state = GameState::MENU_TITLE;

	option_selected = 0;
	option_confirmed = false;

	timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

	setup_menu_shape_particles();
}

void Game::setup_menu_settings() {
	game_state = GameState::MENU_SETTINGS;

	option_selected = 0;
	option_confirmed = false;

	timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

	//setup_menu_shape_particles();
}

void Game::setup_menu_level_select() {
	game_state = GameState::MENU_LEVEL_SELECT;

	option_selected = 0;
	option_confirmed = false;

	timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

	timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::PAUSED);
	timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

	//setup_menu_shape_particles();
}

void Game::setup_game_running(uint8_t level_number) {
	game_state = GameState::GAME_RUNNING;

	paused = false;

	level_handler.load_level(asset_levels[level_number]);
	current_level = level_number;

	player = Player(level_handler.level_spawn_blue_x, level_handler.level_spawn_blue_y, level_handler.level_spawn_pink_x, level_handler.level_spawn_pink_y);

	timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

	timer_handler.set_timer_state(TIMER_ID::GAME_DURATION, TimerState::PAUSED);
	timer_handler.reset_timer(TIMER_ID::GAME_DURATION);

	// reset finish particle timer??

	// Clear spare particle handler
	particle_handlers.spare.clear();

	fade_state = FadeState::UNFADE;
}

void Game::setup_game_paused() {
	game_state = GameState::GAME_PAUSED;

	option_selected = 0;
	option_confirmed = false;

	timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

	//timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
	//timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

	//fade_state = FadeState::UNFADE;
	
	//pause_transition.open();
}

void Game::setup_game_end() {
	game_state = GameState::GAME_END;

	option_selected = 0;
	option_confirmed = false;

	timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

	timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

	// Clear spare particle handler
	particle_handlers.spare.clear();

	fade_state = FadeState::UNFADE;
}


void Game::resume_game_running() {
	game_state = GameState::GAME_RUNNING;

	//timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
	//timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

	//fade_state = FadeState::UNFADE;

	// Reset space to stop players jumping as soon as fade ends
	input_handler.set_key(KeyHandler::Key::SPACE, KeyHandler::KeyState::STILL_UP);

	pause_transition.open();

	paused = false;
}

void Game::restart_game() {
	level_handler.load_level(asset_levels[current_level]);

	player = Player(level_handler.level_spawn_blue_x, level_handler.level_spawn_blue_y, level_handler.level_spawn_pink_x, level_handler.level_spawn_pink_y);

	timer_handler.set_timer_state(TIMER_ID::GAME_DURATION, TimerState::PAUSED);
	timer_handler.reset_timer(TIMER_ID::GAME_DURATION);

	// reset finish particle timer??

	// Clear spare particle handler
	//particle_handlers.spare.clear();


	//timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::RUNNING);
	//timer_handler.reset_timer(TIMER_ID::MENU_TRANSITION_FADE);

	//fade_state = FadeState::UNFADE;
}


void Game::render_fade_in_rect(float delay) {
	if (timer_handler.get_timer_state(TIMER_ID::MENU_TRANSITION_FADE) == TimerState::RUNNING) {
		if (timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) <= delay) {
			// Calculate alpha
			uint8_t alpha = 0xFF * (1.0f - timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) / delay);

			// Fill with semi-transparent black
			render_fade_rect(alpha);
		}
		else {
			timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::PAUSED);
		}
	}
	else {
		fade_state = FadeState::NONE;
	}
}

void Game::render_fade_out_rect(float delay) {
	if (timer_handler.get_timer_state(TIMER_ID::MENU_TRANSITION_FADE) == TimerState::RUNNING) {
		if (timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) <= delay) {
			// Calculate alpha
			uint8_t alpha = 0xFF * (timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE) / delay);

			// Fill with semi-transparent black
			render_fade_rect(alpha);
			//printf("fade %u, time %f, delay %f\n", alpha, timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE), delay);
		}
		else {
			timer_handler.set_timer_state(TIMER_ID::MENU_TRANSITION_FADE, TimerState::PAUSED);
		}
	}
	else {
		fade_state = FadeState::NONE;
	}
}

void Game::render_fade_rect(uint8_t alpha) {
	SDL_Rect screen_rect{ 0, 0, WINDOW::WIDTH, WINDOW::HEIGHT };

	// Fill with semi-transparent black
	SDL_SetRenderDrawColor(renderer, Colour(COLOURS::BLACK, alpha));
	SDL_RenderFillRect(renderer, &screen_rect);
}

std::pair<float, float> Game::get_bezier_text_positions() {
	float left_x, right_x;
	left_x = right_x = WINDOW::TEXT_SCALED_WIDTH_HALF;

	if (timer_handler.get_timer_state(TIMER_ID::MENU_BEZIER_TEXT) == TimerState::RUNNING || option_confirmed) {
		// Modify the x value because bezier curves are running
		// If option_confirmed, then bezier curves may not be running, but if they aren't, the text should be off the screen anyways

		float ratio = timer_handler.get_timer(TIMER_ID::MENU_BEZIER_TEXT) / DELAY::MENU_BEZIER_LENGTH;

		if (!option_confirmed) {
			// Entering rather than leaving
			ratio = 1 - ratio;
		}

		left_x = BEZIER::bezier_x(BEZIER::FROM_LEFT, ratio);
		right_x = BEZIER::bezier_x(BEZIER::FROM_RIGHT, ratio);
	}

	return std::pair<float, float>(left_x, right_x);
}



void Game::handle_menu_shape_particles() {
	// Handle shape particles in background
	if (timer_handler.get_timer(TIMER_ID::MENU_SHAPE_GENERATION) >= DELAY::MENU_SHAPE_GENERATION) {
		timer_handler.reset_timer(TIMER_ID::MENU_SHAPE_GENERATION);

		particle_handlers.back.add(create_menu_shape_particle());
	}

	// Remove particles if they're off the screen
	particle_handlers.back.remove_if([](ImageParticle& particle) { return particle.get_y() * particle.get_scale() > WINDOW::HEIGHT * 1.1f; });
}

ImageParticle Game::create_menu_shape_particle() {
	menu_shape_particle_count++;

	uint8_t colour = rand() % 3 ? 1 : 0;
	//uint8_t colour = rand() % 2;

	float scale = 3.0f + (rand() % 17) / 4.0f + colour;

	float y_speed = 2 + colour * 4 + rand() % 2;

	uint16_t x = WINDOW::WIDTH * (menu_shape_particle_count % MENU::SHAPE_PARTICLE::CHANNELS) / (MENU::SHAPE_PARTICLE::CHANNELS * scale);

	return ImageParticle(TILE_ID::PARTICLE::SQUARE_DARK + colour, x, -SPRITES::SIZE, (rand() % 5 - 2), y_speed, 0.0f, 0.0f, 0.0f, rand() % 90 - 45, scale);
}

ImageParticle Game::create_game_finish_particle(float x, float y, uint8_t colour) {
	//game_finish_particle_count++;

	//uint8_t colour = rand() % 3 ? 1 : 0;
	//uint8_t colour = rand() % 2;

	float scale = 1.0f + (rand() % 11) / 10.0f;

	float position_scale = SPRITES::SCALE / scale;

	float x_speed = (rand() % 7 - 3);
	float y_speed = -10.0f;

	float new_x = x + rand() % (uint8_t)(SPRITES::SIZE - SPRITES::SIZE_HALF / position_scale);

	float fade = -40.0f;

	return ImageParticle(TILE_ID::PARTICLE::FINISH_BLUE + colour, new_x * position_scale, y * position_scale, x_speed * position_scale, y_speed * position_scale, 0.0f, 0.0f, 0.0f, rand() % 90 - 45, scale, 255, fade);
}

void Game::fill_menu_shape_particle(uint8_t count) {
	for (uint8_t i = 0; i < count; i++) {
		//uint8_t colour = rand() % 3 ? 1 : 0;
		uint8_t colour = rand() % 2;

		float scale = 3.0f + (rand() % 17) / 4.0f + colour;

		float y_speed = 2 + colour * 4 + rand() % 2;

		uint16_t x = WINDOW::WIDTH * (menu_shape_particle_count % MENU::SHAPE_PARTICLE::CHANNELS) / (MENU::SHAPE_PARTICLE::CHANNELS * scale);
		uint16_t y = WINDOW::HEIGHT * (menu_shape_particle_count % count) / (count * scale);

		x -= rand() % SPRITES::SIZE - SPRITES::SIZE_HALF;
		y -= (rand() % SPRITES::SIZE - SPRITES::SIZE_HALF) * scale / 4.0f;

		particle_handlers.back.add(ImageParticle(TILE_ID::PARTICLE::SQUARE_DARK + colour, x, y, (rand() % 5 - 2), y_speed, 0.0f, 0.0f, 0.0f, rand() % 90 - 45, scale));

		menu_shape_particle_count++;
	}
}

void Game::setup_menu_shape_particles() {
	if (!particle_handlers.back.count_particles()) {
		timer_handler.set_timer_state(TIMER_ID::MENU_SHAPE_GENERATION, TimerState::RUNNING);
		timer_handler.reset_timer(TIMER_ID::MENU_SHAPE_GENERATION);

		fill_menu_shape_particle(MENU::SHAPE_PARTICLE::COUNT);
	}
}

bool Game::level_is_completed() {
	// TODO: consider delegating to LevelHandler


	// Both are on finish
	/*return (is_colliding_with_finish(level_handler.level_finish_x, level_handler.level_finish_y, player.get_blue_x(), player.get_blue_y(), level_handler.sprite_size) &&
		is_colliding_with_finish(level_handler.level_finish_x, level_handler.level_finish_y, player.get_pink_x(), player.get_pink_y(), level_handler.sprite_size));*/

	//return false;

	// Could be improved
	return level_handler.level_finish_blue_x == player.get_blue_x() && level_handler.level_finish_blue_y - GAME::FINISH::HEIGHT == player.get_blue_y() &&
		level_handler.level_finish_pink_x == player.get_pink_x() && level_handler.level_finish_pink_y - GAME::FINISH::HEIGHT == player.get_pink_y();
}


void Game::handle_music() {
	// TODO: possibly change settings stuff to fade out/in
	// TODO:	...OR... change settings stuff to set volume, so it still plays in background (also, fade in/out if possible)

	if (data.audio_music) {
		// NOTE: toggling settings often gets weird cutin/outs at start
		if (!audio_handler.is_music_playing()) {
			// Assumes at least 2 pieces of music in music_samples
			audio_handler.play_music(audio_handler.music_samples.at(1 + rand() % (audio_handler.music_samples.size() - 1)), 0);
		}
	}
	else if (audio_handler.is_music_playing()) {
		// Don't play music - stop it if it's running!
		audio_handler.fade_music_out(AUDIO::MUSIC_FADE_TIME);
	}
}



void Game::load_save_data(std::string assets_path) {
	JSONHandler::json json_data = JSONHandler::read(assets_path + FILES::SAVE_DATA);
	try {
		data.level_reached = json_data.at("level_reached").get<uint8_t>();
	}
	catch (const JSONHandler::type_error& error) {
		printf("Invalid data, using defaults...\n");
		data.level_reached = 0;
	}
}

void Game::write_save_data(std::string assets_path) {
	JSONHandler::json json_data;
	json_data["level_reached"] = data.level_reached;
	JSONHandler::write(assets_path + FILES::SAVE_DATA, json_data);
}




SDL_Texture* Game::load_texture(std::string path, bool display_errors) {
	// Load image at specified path
	SDL_Surface* image = IMG_Load(path.c_str());

	if (image == NULL && display_errors)
	{
		printf("Unable to create texture from %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Create texture from image
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	if (texture == NULL && display_errors)
	{
		printf("Unable to create texture from %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(image);

	return texture;
}

SDL_Surface* Game::load_surface(std::string path, bool display_errors) {
	// Load image at specified path
	SDL_Surface* image = IMG_Load(path.c_str());

	if (image == NULL && display_errors)
	{
		printf("Unable to load image %s!\nSDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}

	return image;
}



void pause_transition_update(TransitionState* transition_state, float* timer, float dt) {
	*timer += dt;

	if (*transition_state == TransitionState::OPENING) {
		if (*timer > DELAY::PAUSE_FADE_LENGTH) {
			*transition_state = TransitionState::OPEN;
		}
	}
	else if (*transition_state == TransitionState::CLOSING) {
		if (*timer > DELAY::PAUSE_FADE_LENGTH) {
			*transition_state = TransitionState::CLOSED;
		}
	}
}

void pause_transition_render(TransitionState* transition_state, float* timer, SDL_Renderer* renderer, Spritesheet& spritesheet) {
	SDL_Rect screen_rect{ 0, 0, WINDOW::WIDTH, WINDOW::HEIGHT };

	if (*transition_state == TransitionState::OPENING) {
		// Calculate alpha
		uint8_t alpha = MENU::PAUSED_BACKGROUND_ALPHA * (1.0f - *timer / DELAY::PAUSE_FADE_LENGTH);

		// Fill with semi-transparent black
		SDL_SetRenderDrawColor(renderer, Colour(COLOURS::BLACK, alpha));
		SDL_RenderFillRect(renderer, &screen_rect);

		//printf("fade %u, time %f, delay %f\n", alpha, timer_handler.get_timer(TIMER_ID::MENU_TRANSITION_FADE), delay);
	}
	else if (*transition_state == TransitionState::CLOSING) {
		// Calculate alpha
		uint8_t alpha = MENU::PAUSED_BACKGROUND_ALPHA * (*timer / DELAY::PAUSE_FADE_LENGTH);

		// Fill with semi-transparent black
		SDL_SetRenderDrawColor(renderer, Colour(COLOURS::BLACK, alpha));
		SDL_RenderFillRect(renderer, &screen_rect);
	}
	else if (*transition_state == TransitionState::CLOSED) {
		// In this case, don't fill
		// Normally you'd want to fill with black when closed, but pause menu is 'closed' the whole time.

		/*SDL_SetRenderDrawColor(renderer, Colour(COLOURS::BLACK, 0xFF));
		SDL_RenderFillRect(renderer, &screen_rect);*/
	}
}