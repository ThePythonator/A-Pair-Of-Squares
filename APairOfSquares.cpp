#include "APairOfSquares.hpp"

// Globals

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

// Methods

bool init() {
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

void quit() {
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

void load_data() {
	// Load data such as images
	spritesheet_texture = load_texture("F:/Repositories/APairOfSquares/assets/spritesheet.png");
	font_sheet_texture = load_texture("F:/Repositories/APairOfSquares/assets/font.png");

	spritesheet = Spritesheet(renderer, spritesheet_texture, SPRITE_SIZE, SPRITE_SCALE);

	font = FontHandler::Font(renderer, font_sheet_texture, SPRITE_SIZE, SPRITE_SCALE);

	// Load timers
	TIMER_ID::INTRO_FADE = timer_handler.add_timer();
	//TIMER_ID::BLAH = timer_handler.add_timer();
}

void clear_data() {
	// Free loaded data such as images
	SDL_DestroyTexture(spritesheet_texture);
	spritesheet_texture = NULL;

	SDL_DestroyTexture(font_sheet_texture);
	font_sheet_texture = NULL;
}

int main(int argc, char* argv[])
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
		//SDL_SetRenderDrawColor(renderer, 0x03, 0x07, 0x10, 0xFF);
		SDL_SetRenderDrawColor(renderer, 0xF3, 0xE7, 0xD0, 0xFF);
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

void update(float dt) {
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

void render() {
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

void update_menu_intro(float dt) {
	if (timer_handler.get_timer(TIMER_ID::INTRO_FADE) >= DELAY::INTRO_LENGTH) {
		// Time to end intro and switch to title screen
		game_state = GameState::MENU_TITLE;

		// Reset timer ready for use in unfading to title screen
		timer_handler.reset_timer(TIMER_ID::INTRO_FADE);
	}
}

void render_menu_intro() {
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

void update_menu_title(float dt) {
	// to remove
	//player.update(input_handler, dt);
}

void render_menu_title() {
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

	TextHandler::render_text(font, "TESTING!", 10, 10);
}

SDL_Texture* load_texture(std::string path) {
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