#include "TextManager.hpp"

FontHandler::Font::Font() {
	// Generate empty character_rects
	for (uint8_t i = 0; i < ALPHABET_LENGTH; i++) {
		character_rects[i] = SDL_Rect{ 0 };
	}
}

FontHandler::Font::Font(SDL_Renderer* renderer, SDL_Texture* font_sheet_texture, uint8_t sprite_size, uint8_t scale) {
	font_sheet = Spritesheet(renderer, font_sheet_texture, sprite_size, scale);

	// Generate character_rects
	for (uint8_t i = 0; i < ALPHABET_LENGTH; i++) {
		// change this to actually look for bars, and get correct height
		character_rects[i] = SDL_Rect{ (i % 32) * sprite_size, (i / 32) * sprite_size, sprite_size, sprite_size };
	}
}

void FontHandler::Font::render_char(uint8_t c, float x, float y) {
	// Check character is one we have an image/rect for
	if (c >= 32 && c <= 255) {
		// Render character
		font_sheet.rect_scaled(&character_rects[c - 32], x, y);
	}
}

SDL_Rect* FontHandler::Font::get_character_rect(uint8_t c) {
	// Check character is one we have a rect for
	if (c >= 32 && c <= 255) {
		// Render character
		return &character_rects[c - 32];
	}
	else {
		SDL_Rect empty_rect{ 0 };
		return &empty_rect;
	}
}



void TextHandler::render_text(FontHandler::Font& font, std::string text, float x, float y, int8_t space_width, AnchorPosition anchor) {
	uint16_t width = 0;
	uint8_t height = 0;

	for (uint8_t c : text) {
		// Get width of that character and update width
		SDL_Rect* char_rect = font.get_character_rect(c);

		// Update width and height
		width += char_rect->w + space_width;
		height = char_rect->h;
	}
	// In the above loop, we've offset one space too many (we need one less space than the number of characters)
	width -= space_width;

	float current_x = 0;
	float current_y = 0;

	// Handle positioning
	if (anchor & AnchorPosition::LEFT) {
		 current_x = x;
	}
	else if (anchor & AnchorPosition::RIGHT) {
		current_x = x - width;
	}
	else {
		current_x = x - (width / 2);
	}

	if (anchor & AnchorPosition::TOP) {
		current_y = y;
	}
	else if (anchor & AnchorPosition::BOTTOM) {
		current_y = y - height;
	}
	else {
		current_y = y - (height / 2);
	}

	for (uint8_t c : text) {
		// Render character
		font.render_char(c, current_x, current_y);

		// Get width of that character and update current_x
		SDL_Rect* char_rect = font.get_character_rect(c);
		current_x += char_rect->w + space_width;
	}
}