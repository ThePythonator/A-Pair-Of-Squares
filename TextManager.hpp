#pragma once

#include "SDL.h"

#include <string>

#include "Spritesheet.hpp"

namespace FontHandler {
	const uint8_t ALPHABET_LENGTH = 96;

	class Font {
	public:
		Font();
		Font(SDL_Renderer* renderer, SDL_Texture* font_sheet_texture, uint8_t sprite_size = 16, uint8_t scale = 1);

		void render_char(uint8_t c, float x, float y);

	protected:
		Spritesheet font_sheet;

		SDL_Rect character_rects[ALPHABET_LENGTH];
	};
}

namespace TextHandler {
	enum class AnchorPosition {
		CENTER_X		= 0x000001,
		CENTER_Y		= 0x000010,

		LEFT			= 0x000100,
		RIGHT			= 0x001000,
		TOP				= 0x010000,
		BOTTOM			= 0x100000,

		CENTER_CENTER	= CENTER_X & CENTER_Y,

		TOP_RIGHT		= TOP & RIGHT,
		TOP_LEFT		= TOP & LEFT,
		BOTTOM_RIGHT	= BOTTOM & RIGHT,
		BOTTOM_LEFT		= BOTTOM & LEFT
	};

	void render_text(FontHandler::Font& font, std::string text, float x, float y, AnchorPosition anchor = AnchorPosition::CENTER_CENTER);
}