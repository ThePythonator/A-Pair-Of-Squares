#pragma once

#include "SDL.h"

#include <string>

#include "Spritesheet.hpp"

namespace FontHandler {
	const uint8_t ALPHABET_LENGTH = 96;

	class Font {
	public:
		Font();
		Font(SDL_Renderer* renderer, SDL_Texture* font_sheet_texture, SDL_Surface* font_sheet_surface, uint8_t sprite_size = 16, uint8_t scale = 1);

		void render_char(uint8_t c, float x, float y);
		SDL_Rect get_character_rect(uint8_t c);

	private:
		Spritesheet font_sheet;

		SDL_Rect character_rects[ALPHABET_LENGTH];
	};
}

namespace TextHandler {
	enum AnchorPosition {
		LEFT			= 0b0100,
		CENTER_X		= 0b0000,
		RIGHT			= 0b1000,

		TOP				= 0b0001,
		CENTER_Y		= 0b0000,
		BOTTOM			= 0b0010,

		TOP_RIGHT		= TOP | RIGHT,
		TOP_CENTER		= TOP | CENTER_X,
		TOP_LEFT		= TOP | LEFT,

		CENTER_RIGHT	= CENTER_Y | RIGHT,
		CENTER_CENTER	= CENTER_X | CENTER_Y,
		CENTER_LEFT		= CENTER_Y | LEFT,

		BOTTOM_RIGHT	= BOTTOM | RIGHT,
		BOTTOM_CENTER	= BOTTOM | CENTER_X,
		BOTTOM_LEFT		= BOTTOM | LEFT
	};

	void render_text(FontHandler::Font& font, std::string text, float x, float y, int8_t space_width = 0, AnchorPosition anchor = AnchorPosition::CENTER_CENTER);
}