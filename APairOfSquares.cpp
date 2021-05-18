#include "APairOfSquares.hpp"

// Constants

const uint16_t WINDOW_WIDTH = 640;
const uint16_t WINDOW_HEIGHT = 360;

const char* APP_TITLE = "A Pair of Squares";

// Globals

// Main game window
SDL_Window* window = NULL;

// Main game canvas/screen
SDL_Surface* screen = NULL;
SDL_Surface* actual_screen = NULL;

// Rect used to scale screen to actual_screen
SDL_Rect scale_rect{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

// Methods

bool init() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		// Create window
		window = SDL_CreateWindow(
			APP_TITLE,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN
		);

		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			// Get window surface
			actual_screen = SDL_GetWindowSurface(window);

			// TODO: get screen, needs to be same as actual_screen but small (e.g. 320x180)
			//screen = ???
		}
	}

	return true;
}

void quit() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	// Initialise SDL and globals
	init();
    

    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    //SDL_RenderClear(renderer);
    //SDL_RenderPresent(renderer);

	bool running = true;
	SDL_Event sdl_event;

	// Main game loop
	while (running) {
		// Handle events
		while (SDL_PollEvent(&sdl_event) != 0)
		{
			if (sdl_event.type == SDL_QUIT)
			{
				// X is pressed
				running = false;
			}
			else if (sdl_event.type == SDL_KEYDOWN)
			{
				// User has pressed a key

				// Check which key is pressed
				switch (sdl_event.key.keysym.sym)
				{
				default:
					break;
				}
			}
			else if (sdl_event.type == SDL_KEYUP) {

			}
		}

		//update(dt);
		render();

		// Scale the screen to the actual window surface
		SDL_BlitScaled(screen, NULL, actual_screen, &scale_rect);

		// Update the surface
		SDL_UpdateWindowSurface(window);
	}

	quit();

    return 0;
}

void update(float dt) {

}

void render() {
	// Fill the surface
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x03, 0x07, 0x10));
}

SDL_Surface* load_image(std::string path)
{
	// The final converted image
	SDL_Surface* converted_image = NULL;

	// Load image at specified path
	SDL_Surface* image = SDL_LoadBMP(path.c_str());

	// Convert image to screen format
	converted_image = SDL_ConvertSurface(image, actual_screen->format, 0);

	// Get rid of old loaded surface
	SDL_FreeSurface(image);

	return converted_image;
}