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
	// Check character is one we have an image for
	if (c >= 32 && c <= 255) {
		// Render character
		font_sheet.rect_scaled(&character_rects[c - 32], x, y);
	}
}

//TODO
void TextHandler::render_text(FontHandler::Font& font, std::string text, float x, float y, AnchorPosition anchor) {
	// Crude method for now (to change) - ignores centering and spacing
	for (char c : text) {
		// eww
		font.render_char(c, x, y);
		x += 16;
	}
}