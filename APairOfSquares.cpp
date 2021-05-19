#include "APairOfSquares.hpp"

// Constants

const uint16_t WINDOW_WIDTH = 960;
const uint16_t WINDOW_HEIGHT = 640; //540

const char* APP_TITLE = "A Pair of Squares";

// Globals

// Main game window
SDL_Window* window = NULL;

// Main game canvas/screen
//SDL_Surface* screen = NULL;

// Renderer for window
SDL_Renderer* renderer = NULL;

// Spritesheet
SDL_Texture* spritesheet_texture = NULL;
Spritesheet spritesheet;

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

	spritesheet = Spritesheet(renderer, spritesheet_texture);
}

void clear_data() {
	// Free loaded data such as images
	SDL_DestroyTexture(spritesheet_texture);
	spritesheet_texture = NULL;
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

	// Main game loop
	while (running) {
		// Handle events
		while (SDL_PollEvent(&sdl_event) != 0) {
			if (sdl_event.type == SDL_QUIT) {
				// X is pressed
				running = false;
			}
			else if (sdl_event.type == SDL_KEYDOWN) {
				// User has pressed a key

				// Check which key is pressed
				switch (sdl_event.key.keysym.sym)
				{
				default:
					break;
				}
			}
			else if (sdl_event.type == SDL_KEYUP) {
				// User has released a key
			}
		}

		// Calculate dt
		time = SDL_GetTicks();
		dt = time / 1000.0f;
		last_time = time;

		update(dt);

		// Clear the screen
		SDL_RenderClear(renderer);

		// Render game
		render();

		// Update screen
		SDL_RenderPresent(renderer);
	}

	// Quit everything
	quit();

	return 0;
}

void update(float dt) {

}

void render() {
	spritesheet.sprite(0, 32, 16, 4);

	spritesheet.sprite(4, 128, 16, 4);


	spritesheet.sprite(80, 16, 96, 4);
	spritesheet.sprite(81, 80, 96, 4);
	spritesheet.sprite(82, 144, 96, 4);
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