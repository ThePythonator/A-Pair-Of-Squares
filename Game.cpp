#include "Game.hpp"

// Constants

namespace SPRITES {
	const uint8_t SCALE = 4;

	const uint8_t SIZE = 16;
	const uint8_t SIZE_HALF = SIZE / 2;


	const uint16_t SQUARE_PARTICLE = 228;
}

namespace WINDOW {
	const uint16_t WIDTH = 960;
	const uint16_t HEIGHT = 640; //540

	const uint16_t SCALED_WIDTH = WIDTH / SPRITES::SCALE;
	const uint16_t SCALED_HEIGHT = HEIGHT / SPRITES::SCALE;

	const uint16_t SCALED_WIDTH_HALF = SCALED_WIDTH / 2;
	const uint16_t SCALED_HEIGHT_HALF = SCALED_HEIGHT / 2;

	const uint8_t FPS = 120;
	//const float MIN_DT = 1.0f / FPS;
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

	const uint8_t SHAPE_PARTICLE_COUNT = 7;
	const uint8_t SHAPE_PARTICLE_CHANNELS = 4;
}

namespace COLOURS {
	const Colour BLACK = Colour(0x04, 0x07, 0x10);
	const Colour WHITE = Colour(0xFF, 0xFF, 0xE4);

	const Colour SELECTED = Colour(0x1C, 0x92, 0xA7);
}

namespace STRINGS {
	const std::string APP_TITLE = "A Pair of Squares";

	const std::string ON = "On";
	const std::string OFF = "Off";

	const std::string CLN_SPC = ": ";

	namespace MENU {
		namespace TITLE {
			const std::string OPTION_PLAY = "Play";
			const std::string OPTION_SETTINGS = "Settings";
			const std::string OPTION_QUIT = "Quit";
		}

		namespace SETTINGS {
			const std::string OPTION_MUSIC = "Music";
			const std::string OPTION_SFX = "SFX";
			const std::string OPTION_BACK = "Back";
		}
	}
}

namespace FILES {
	const std::string SPRITESHEET = "spritesheet.png";

	const std::string FONT_SHEET = "another-font.png";
	// const std::string FONT_SHEET = "font.png";
}

namespace DELAY {
	const float TRANSITION_FADE_LENGTH = 0.6f;

	const float MENU_INTRO_FADE_START = 2.0f;
	const float MENU_INTRO_FADE_LENGTH = 1.0f;
	const float MENU_INTRO_LENGTH = MENU_INTRO_FADE_START + MENU_INTRO_FADE_LENGTH;

	const float MENU_BEZIER_LENGTH = 1.0f;

	const float MENU_SHAPE_GENERATION = 2.5f;
}

// Timer IDs (are set later in program)
namespace TIMER_ID {
	const uint8_t UNINITIALISED = 255;

	uint8_t MENU_INTRO_FADE = UNINITIALISED;
	uint8_t MENU_BEZIER_TEXT = UNINITIALISED;
	uint8_t MENU_SHAPE_GENERATION = UNINITIALISED;
}

// Nodes for bezier transitions
namespace BEZIER {
	const uint8_t MENU_BEZIER_NODE_COUNT = 3;

	// 1st Node is doubled to make transitions ease in/out more smoothly
	const std::vector<Node> FROM_LEFT{ Node{WINDOW::SCALED_WIDTH_HALF, 0}, Node{WINDOW::SCALED_WIDTH_HALF, 0}, Node{WINDOW::SCALED_WIDTH, 0}, Node{-WINDOW::SCALED_WIDTH, 0} };
	const std::vector<Node> FROM_RIGHT{ Node{WINDOW::SCALED_WIDTH_HALF, 0}, Node{WINDOW::SCALED_WIDTH_HALF, 0}, Node{0, 0}, Node{WINDOW::SCALED_WIDTH * 2, 0} };

	float bezier_x(std::vector<Node> nodes, float ratio) {
		return bezier_curve(nodes, ratio).first;
	}
}


Game::Game() {

}

// Methods

bool Game::init() {
	// Initialise SDL
	int sdl_init_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_init_result < 0)
	{
		printf("SDL could not initialize!\nSDL Error: %s\n", SDL_GetError());
		return false;
	}

	// Initialise SDL_image
	int img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int img_init_result = IMG_Init(img_init_flags);
	if ((img_init_result & img_init_flags) != img_init_flags) {
		printf("SDL_IMG could not initialize!\nSDL_IMG Error: %s\n", IMG_GetError());
		return false;
	}

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
	// Destroy assets
	clear_data();

	// Destroy renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	// Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL
	IMG_Quit();
	SDL_Quit();
}

void Game::load_data() {
	// Uses spritesheet_texture as a test to check whether we can find the assets folder.
	std::string assets_path = find_assets_path(FILES::SPRITESHEET);

	// Load data such as images
	spritesheet_texture = load_texture(assets_path + FILES::SPRITESHEET);
	font_sheet_texture = load_texture(assets_path + FILES::FONT_SHEET);

	spritesheet = Spritesheet(renderer, spritesheet_texture, SPRITES::SIZE, SPRITES::SCALE);

	SDL_Surface* font_sheet_surface = load_surface(assets_path + FILES::FONT_SHEET);

	//font_black = FontHandler::Font(renderer, font_sheet_texture, font_sheet_surface, SPRITES::SIZE, SPRITE_SCALE, COLOURS::BLACK);
	font_white = FontHandler::Font(renderer, font_sheet_texture, font_sheet_surface, SPRITES::SIZE, SPRITES::SCALE, COLOURS::WHITE);
	font_selected = FontHandler::Font(renderer, font_sheet_texture, font_sheet_surface, SPRITES::SIZE, SPRITES::SCALE, COLOURS::SELECTED);

	SDL_FreeSurface(font_sheet_surface);

	// Load timers
	TIMER_ID::MENU_INTRO_FADE = timer_handler.add_timer();
	TIMER_ID::MENU_BEZIER_TEXT = timer_handler.add_timer();
	TIMER_ID::MENU_SHAPE_GENERATION = timer_handler.add_timer();
}

void Game::clear_data() {
	// Free loaded data such as images
	SDL_DestroyTexture(spritesheet_texture);
	spritesheet_texture = NULL;

	SDL_DestroyTexture(font_sheet_texture);
	font_sheet_texture = NULL;
}

std::string Game::find_assets_path(std::string test_file, uint8_t depth) {
	std::string base_path = SDL_GetBasePath();
	SDL_Surface* test_surface = load_surface("assets/" + test_file);

	if (test_surface != NULL) {
		base_path = "";
	}

	uint8_t count = 0;
	while (test_surface == NULL && count < depth) {
		test_surface = load_surface(base_path + "assets/" + test_file);

		if (test_surface == NULL) {
			base_path += "../";
		}

		count++;
	}

	if (test_surface == NULL) {
		return "assets/";
	}

	SDL_FreeSurface(test_surface);

	return base_path + "assets/";
}

uint8_t Game::run()
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

		update(dt);

		// Clear the screen
		SDL_SetRenderDrawColor(renderer, COLOURS::BLACK);
		SDL_RenderClear(renderer);

		// Render game
		render();

		// Update screen
		SDL_RenderPresent(renderer);

		// Limit framerate - to fix
		//if (dt < MIN_DT) {
		//	// Wait remaining time
		//	SDL_Delay(1000 * (MIN_DT - dt));
		//}


		//printf("FPS: %f\n", 1.0f / dt);
	}

	// Quit everything
	quit();

	return 0;
}

void Game::update(float dt) {
	// Update timers
	timer_handler.update(dt);

	// Update particles
	particle_handlers.back.update(dt);
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
	default:
		break;
	}
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
	default:
		break;
	}

	// Render particles
	particle_handlers.front.render(spritesheet);
}

// Update and render functions for each state
void Game::update_menu_intro(float dt) {
	if (timer_handler.get_timer(TIMER_ID::MENU_INTRO_FADE) >= DELAY::MENU_INTRO_LENGTH) {
		// Time to end intro and switch to title screen
		setup_menu_title();

		// Reset timer ready for use in unfading to title screen
		timer_handler.reset_timer(TIMER_ID::MENU_INTRO_FADE);
	}
}

void Game::render_menu_intro() {
	// Display logo
	SDL_Rect src_rect{ 0, 224, 32, 32 };
	spritesheet.rect(&src_rect, WINDOW::SCALED_WIDTH_HALF / 2 - SPRITES::SIZE, WINDOW::SCALED_HEIGHT_HALF / 2 - SPRITES::SIZE, 8);

	// Display fade-out black rect
	if (timer_handler.get_timer(TIMER_ID::MENU_INTRO_FADE) >= DELAY::MENU_INTRO_FADE_START) {
		SDL_Rect screen_rect{ 0, 0, WINDOW::WIDTH, WINDOW::HEIGHT };

		// Calculate alpha
		uint8_t alpha = 255 * (timer_handler.get_timer(TIMER_ID::MENU_INTRO_FADE) - DELAY::MENU_INTRO_FADE_START) / (float)DELAY::MENU_INTRO_FADE_LENGTH;

		// Fill with semi-transparent black
		SDL_SetRenderDrawColor(renderer, Colour(COLOURS::BLACK, alpha));
		SDL_RenderFillRect(renderer, &screen_rect);
	}
}

void Game::update_menu_title(float dt) {
	// Handle shape particles in background
	if (timer_handler.get_timer(TIMER_ID::MENU_SHAPE_GENERATION) >= DELAY::MENU_SHAPE_GENERATION) {
		timer_handler.reset_timer(TIMER_ID::MENU_SHAPE_GENERATION);

		particle_handlers.back.add(create_menu_shape_particle());
	}

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
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.DOWN)) {
			if (option_selected < (uint8_t)MENU::TITLE::OPTION_COUNT - 1) {
				option_selected++;
			}
		}

		// Check if user has just selected an option
		if (KeyHandler::just_down(input_handler.get_key_union().keys.SPACE)) {
			// Reset timer and set it to running
			timer_handler.set_timer_state(TIMER_ID::MENU_BEZIER_TEXT, TimerState::RUNNING);
			timer_handler.reset_timer(TIMER_ID::MENU_BEZIER_TEXT);

			option_confirmed = true;
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
	// Display fade-in black rect
	if (timer_handler.get_timer(TIMER_ID::MENU_INTRO_FADE) <= DELAY::TRANSITION_FADE_LENGTH) {
		SDL_Rect screen_rect{ 0, 0, WINDOW::WIDTH, WINDOW::HEIGHT };

		// Calculate alpha
		uint8_t alpha = 0xFF * (1.0f - timer_handler.get_timer(TIMER_ID::MENU_INTRO_FADE) / DELAY::TRANSITION_FADE_LENGTH);

		// Fill with semi-transparent black
		SDL_SetRenderDrawColor(renderer, Colour(COLOURS::BLACK, alpha));
		SDL_RenderFillRect(renderer, &screen_rect);
	}

	float left_x, right_x;
	left_x = right_x = WINDOW::SCALED_WIDTH_HALF;

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

	
	TextHandler::render_text(option_selected == 0 ? font_selected : font_white, STRINGS::MENU::TITLE::OPTION_PLAY, left_x, WINDOW::SCALED_HEIGHT_HALF - SPRITES::SIZE * 2, 1);
	TextHandler::render_text(option_selected == 1 ? font_selected : font_white, STRINGS::MENU::TITLE::OPTION_SETTINGS, right_x, WINDOW::SCALED_HEIGHT_HALF, 1);
	TextHandler::render_text(option_selected == 2 ? font_selected : font_white, STRINGS::MENU::TITLE::OPTION_QUIT, left_x, WINDOW::SCALED_HEIGHT_HALF + SPRITES::SIZE * 2, 1);
}

void Game::update_menu_settings(float dt) {
	// Handle shape particles in background
	if (timer_handler.get_timer(TIMER_ID::MENU_SHAPE_GENERATION) >= DELAY::MENU_SHAPE_GENERATION) {
		timer_handler.reset_timer(TIMER_ID::MENU_SHAPE_GENERATION);

		particle_handlers.back.add(create_menu_shape_particle());
	}

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
			}
		}
		if (KeyHandler::just_down(input_handler.get_key_union().keys.DOWN)) {
			if (option_selected < (uint8_t)MENU::SETTINGS::OPTION_COUNT - 1) {
				option_selected++;
			}
		}

		// Check if user has just selected an option
		if (KeyHandler::just_down(input_handler.get_key_union().keys.SPACE)) {
			// Handle settings/back
			switch ((MENU::SETTINGS)option_selected)
			{
			case MENU::SETTINGS::MUSIC:
				settings.audio_music = !settings.audio_music;
				break;

			case MENU::SETTINGS::SFX:
				settings.audio_sfx = !settings.audio_sfx;
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

			switch ((MENU::SETTINGS)option_selected)
			{
			case MENU::SETTINGS::MUSIC:
				//setup_menu_level_select();
				break;

			case MENU::SETTINGS::SFX:
				//setup_menu_settings();
				break;

			case MENU::SETTINGS::BACK:
				setup_menu_title();
				break;

			default:
				break;
			}
		}
	}
}

void Game::render_menu_settings() {
	float left_x, right_x;
	left_x = right_x = WINDOW::SCALED_WIDTH_HALF;

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


	TextHandler::render_text(option_selected == 0 ? font_selected : font_white, STRINGS::MENU::SETTINGS::OPTION_MUSIC + STRINGS::CLN_SPC + (settings.audio_music ? STRINGS::ON : STRINGS::OFF), left_x, WINDOW::SCALED_HEIGHT_HALF - SPRITES::SIZE * 2, 1);
	TextHandler::render_text(option_selected == 1 ? font_selected : font_white, STRINGS::MENU::SETTINGS::OPTION_SFX + STRINGS::CLN_SPC + (settings.audio_sfx ? STRINGS::ON : STRINGS::OFF), right_x, WINDOW::SCALED_HEIGHT_HALF, 1);
	TextHandler::render_text(option_selected == 2 ? font_selected : font_white, STRINGS::MENU::SETTINGS::OPTION_BACK, left_x, WINDOW::SCALED_HEIGHT_HALF + SPRITES::SIZE * 2, 1);
}

void Game::update_menu_level_select(float dt) {

}

void Game::render_menu_level_select() {

}

void Game::setup_menu_intro() {
	game_state = GameState::MENU_INTRO;

	timer_handler.set_timer_state(TIMER_ID::MENU_INTRO_FADE, TimerState::RUNNING);
	timer_handler.reset_timer(TIMER_ID::MENU_INTRO_FADE);
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

	setup_menu_shape_particles();
}

void Game::setup_menu_level_select() {
	game_state = GameState::MENU_LEVEL_SELECT;
}

ImageParticle Game::create_menu_shape_particle() {
	shape_particle_count++;

	float scale = 3.0f + (rand() % 21) / 4.0f;

	uint8_t colour = rand() % 2;

	float y_speed = 2 + colour * 4 + rand() % 2;

	uint16_t x = WINDOW::WIDTH * (shape_particle_count % MENU::SHAPE_PARTICLE_CHANNELS) / (MENU::SHAPE_PARTICLE_CHANNELS * scale);

	return ImageParticle(SPRITES::SQUARE_PARTICLE + colour, x, -SPRITES::SIZE, (rand() % 5 - 2), y_speed, 0.0f, 0.0f, 0.0f, rand() % 90 - 45, scale);
}

void Game::fill_menu_shape_particle(uint8_t count) {
	for (uint8_t i = 0; i < count; i++) {
		float scale = 3.0f + (rand() % 21) / 4.0f;

		uint8_t colour = rand() % 2;

		float y_speed = 2 + colour * 4 + rand() % 2;

		uint16_t x = WINDOW::WIDTH * (shape_particle_count % MENU::SHAPE_PARTICLE_CHANNELS) / (MENU::SHAPE_PARTICLE_CHANNELS * scale);
		uint16_t y = WINDOW::HEIGHT * (shape_particle_count % count) / (count * scale);

		x -= rand() % SPRITES::SIZE - SPRITES::SIZE_HALF;
		y -= (rand() % SPRITES::SIZE - SPRITES::SIZE_HALF) * scale / 4.0f;

		particle_handlers.back.add(ImageParticle(SPRITES::SQUARE_PARTICLE + colour, x, y, (rand() % 5 - 2), y_speed, 0.0f, 0.0f, 0.0f, rand() % 90 - 45, scale));

		shape_particle_count++;
	}
}

void Game::setup_menu_shape_particles() {
	if (!particle_handlers.back.count_particles()) {
		timer_handler.set_timer_state(TIMER_ID::MENU_SHAPE_GENERATION, TimerState::RUNNING);
		timer_handler.reset_timer(TIMER_ID::MENU_SHAPE_GENERATION);

		fill_menu_shape_particle(MENU::SHAPE_PARTICLE_COUNT);
	}
}

SDL_Texture* Game::load_texture(std::string path) {
	// Load image at specified path
	SDL_Surface* image = IMG_Load(path.c_str());

	if (image == NULL)
	{
		printf("Unable to create texture from %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Create texture from image
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	if (texture == NULL)
	{
		printf("Unable to create texture from %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(image);

	return texture;
}

SDL_Surface* Game::load_surface(std::string path)
{
	// Load image at specified path
	SDL_Surface* image = IMG_Load(path.c_str());

	if (image == NULL)
	{
		printf("Unable to load image %s!\nSDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}

	return image;
}