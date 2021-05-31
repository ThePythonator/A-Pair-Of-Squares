#include "Game.hpp"

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

namespace DELAY {
	const float TRANSITION_FADE_LENGTH = 0.6f;

	const float INTRO_FADE_START = 2.0f;
	const float INTRO_FADE_LENGTH = 1.0f;
	const float INTRO_LENGTH = INTRO_FADE_START + INTRO_FADE_LENGTH;
}

// Timer IDs (are set later in program)
namespace TIMER_ID {
	const uint8_t UNINITIALISED = 255;

	uint8_t INTRO_FADE = UNINITIALISED;
	//uint8_t BLAH = UNINITIALISED;
}

// Nodes for bezier transitions
namespace BEZIER {
	constexpr std::array<Node, 3> FROM_LEFT(const float y) {
		return { Node{SCALED_WINDOW_WIDTH_HALF, y}, Node{SCALED_WINDOW_WIDTH, y}, Node{-SCALED_WINDOW_WIDTH, y} };
	}

	constexpr std::array<Node, 3> FROM_RIGHT(const float y) {
		return { Node{SCALED_WINDOW_WIDTH_HALF, y}, Node{0, y}, Node{SCALED_WINDOW_WIDTH * 2, y} };
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
		APP_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Set renderer color
	SDL_SetRenderDrawColor(renderer, 0x03, 0x07, 0x10, 0xFF);
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
	std::string assets_path = find_assets_path("spritesheet.png");

	// Load data such as images
	spritesheet_texture = load_texture(assets_path + "spritesheet.png");
	font_sheet_texture = load_texture(assets_path + "font.png");

	spritesheet = Spritesheet(renderer, spritesheet_texture, SPRITE_SIZE, SPRITE_SCALE);

	SDL_Surface* font_sheet_surface = load_surface(assets_path + "font.png");

	font = FontHandler::Font(renderer, font_sheet_texture, font_sheet_surface, SPRITE_SIZE, SPRITE_SCALE);

	SDL_FreeSurface(font_sheet_surface);

	// Load timers
	TIMER_ID::INTRO_FADE = timer_handler.add_timer();
	//TIMER_ID::BLAH = timer_handler.add_timer();
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
	bool running = init();

	load_data();

	// Main game loop variables
	float dt = 0.0f;
	uint32_t last_time = SDL_GetTicks();
	uint32_t time = 0;

	SDL_Event sdl_event;

	// Update input handler (updates all key states etc)
	input_handler.update();

	// Main game loop
	while (running) {
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
		SDL_SetRenderDrawColor(renderer, 0x03, 0x07, 0x10, 0xFF);
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

	switch (game_state) {
	case GameState::MENU_INTRO:
		update_menu_intro(dt);
		break;
	case GameState::MENU_TITLE:
		update_menu_title(dt);
		break;
	default:
		break;
	}
}

void Game::render() {
	switch (game_state) {
	case GameState::MENU_INTRO:
		render_menu_intro();
		break;
	case GameState::MENU_TITLE:
		render_menu_title();
		break;
	default:
		break;
	}
}

// Update and render functions for each state
void Game::update_menu_intro(float dt) {
	if (timer_handler.get_timer(TIMER_ID::INTRO_FADE) >= DELAY::INTRO_LENGTH) {
		// Time to end intro and switch to title screen
		game_state = GameState::MENU_TITLE;

		// Reset timer ready for use in unfading to title screen
		timer_handler.reset_timer(TIMER_ID::INTRO_FADE);
	}
}

void Game::render_menu_intro() {
	// Display logo
	SDL_Rect src_rect{ 0, 224, 32, 32 };
	spritesheet.rect(&src_rect, SCALED_WINDOW_WIDTH_HALF / 2 - SPRITE_SIZE, SCALED_WINDOW_HEIGHT_HALF / 2 - SPRITE_SIZE, 8);

	// Display fade-out black rect
	if (timer_handler.get_timer(TIMER_ID::INTRO_FADE) >= DELAY::INTRO_FADE_START) {
		SDL_Rect screen_rect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

		// Calculate alpha
		uint8_t alpha = 255 * (timer_handler.get_timer(TIMER_ID::INTRO_FADE) - DELAY::INTRO_FADE_START) / (float)DELAY::INTRO_FADE_LENGTH;

		// Fill with semi-transparent black
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, alpha);
		SDL_RenderFillRect(renderer, &screen_rect);
	}
}

void Game::update_menu_title(float dt) {
	// to remove
	//player.update(input_handler, dt);
}

void Game::render_menu_title() {
	// to remove
	player.render(spritesheet);

	// Display fade-in black rect
	if (timer_handler.get_timer(TIMER_ID::INTRO_FADE) <= DELAY::TRANSITION_FADE_LENGTH) {
		SDL_Rect screen_rect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

		// Calculate alpha
		uint8_t alpha = 255 * (1 - timer_handler.get_timer(TIMER_ID::INTRO_FADE) / (float)DELAY::TRANSITION_FADE_LENGTH);

		// Fill with semi-transparent black
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, alpha);
		SDL_RenderFillRect(renderer, &screen_rect);
	}

	//TextHandler::render_text(font, "TESTING!", 0, SCALED_WINDOW_HEIGHT, -3, TextHandler::AnchorPosition::BOTTOM_LEFT);
	TextHandler::render_text(font, "TESTING!", SCALED_WINDOW_WIDTH_HALF, SCALED_WINDOW_HEIGHT_HALF, -2);
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