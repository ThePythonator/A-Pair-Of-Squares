#include "TextManager.hpp"

FontHandler::Font::Font() {
	// Generate empty character_rects
	for (uint8_t i = 0; i < ALPHABET_LENGTH; i++) {
		character_rects[i] = SDL_Rect{ 0, 0, 0, 0 };
	}

	//scale_ratio = 1.0f;
}

FontHandler::Font::Font(SDL_Renderer* renderer, SDL_Surface* font_sheet_surface, uint8_t sprite_size, uint8_t scale, Colour colour, Colour replace_colour) {
	this->colour = colour;
	//this->scale_ratio = (float)sys_scale / (float)scale;

	uint16_t base_x, base_y, x, y, left, right;
	uint8_t r, g, b, a;
	bool found = false;

	//uint8_t pixel_size = font_sheet_surface->format->BytesPerPixel;

	// Generate character_rects
	for (uint8_t i = 0; i < ALPHABET_LENGTH; i++) {
		//character_rects[i] = SDL_Rect{ (i % 32) * sprite_size, (i / 32) * sprite_size, sprite_size, sprite_size };

		base_x = (i % 32) * sprite_size;
		base_y = (i / 32) * sprite_size;

		left = 0;
		right = sprite_size - 1;

		found = false;

		x = y = 0;
		while (!found) {
			// Get pixel at (x, y)
			//SDL_GetRGBA(((uint32_t*)font_sheet_surface->pixels)[(base_y + y) * font_sheet_surface->w + (base_x + x)], font_sheet_surface->format, &r, &g, &b, &a);
			get_pixel(font_sheet_surface, base_x + x, base_y + y, &r, &g, &b, &a);

			if (a) {
				// We found the left edge of the character!
				left = x;
				found = true;
			}

			y++;
			if (y == sprite_size) {
				if (x == sprite_size - 1) {
					// Character is blank
					found = true;
				}

				y = 0;
				x++;
			}
		}

		found = false;

		x = sprite_size - 1;
		y = 0;
		while (!found) {
			// Get pixel at (x, y)
			SDL_GetRGBA(((uint32_t*)font_sheet_surface->pixels)[(base_y + y) * font_sheet_surface->w + (base_x + x)], font_sheet_surface->format, &r, &g, &b, &a);

			if (a) {
				// We found the right edge of the character!
				right = x;
				found = true;
			}

			y++;
			if (y == sprite_size) {
				if (x == 0) {
					// Character is blank
					found = true;
				}

				y = 0;
				x--;
			}
		}

		character_rects[i] = SDL_Rect{ base_x + left, base_y, right - left + 1, sprite_size };
	}


	// Set all pixels to colour
	if (replace_colour.a) {
		for (x = 0; x < 32 * sprite_size; x++) {
			for (y = 0; y < (ALPHABET_LENGTH / 32) * sprite_size; y++) {
				get_pixel(font_sheet_surface, x, y, &r, &g, &b, &a);

				if (a) {
					// Pixel isn't transparent!
					set_pixel(font_sheet_surface, x, y, replace_colour.r, replace_colour.g, replace_colour.b, replace_colour.a);
				}
			}
		}
	}


	// Set up font spritesheet
	SDL_Texture* font_sheet_texture = convert_surface(renderer, font_sheet_surface);

	font_sheet = Spritesheet(renderer, font_sheet_texture, sprite_size, scale);
}

void FontHandler::Font::render_char(uint8_t c, float x, float y) {
	// Check character is one we have an image/rect for (don't include c == 32, since that's a space)
	if (c >= 33 && c <= 255) {
		// Set colour (need to do this every frame since other fonts may also be using the same texture)
		if (colour.a) {
			// Colour isn't transparent
			SDL_SetTextureColorMod(font_sheet.get_texture(), colour);
		}

		// Render character
		font_sheet.rect_scaled(&character_rects[c - 32], x, y);
	}
}

SDL_Rect FontHandler::Font::get_character_rect(uint8_t c) {
	// Check character is one we have a rect for
	if (c >= 32 && c <= 255) {
		// Render character
		return character_rects[c - 32];
	}
	else {
		SDL_Rect empty_rect{ 0 };
		return empty_rect;
	}
}



void TextHandler::render_text(FontHandler::Font& font, std::string text, float x, float y, int8_t space_width, AnchorPosition anchor) {
	uint16_t width = 0;
	uint8_t height = 0;

	for (uint8_t c : text) {
		// Get width of that character and update width
		SDL_Rect char_rect = font.get_character_rect(c);

		// Update width and height
		width += char_rect.w + space_width;
		height = char_rect.h;
	}
	// In the above loop, we've offset one space too many (we need one less space than the number of characters)
	width -= space_width;

	float current_x = 0.0f;
	float current_y = 0.0f;

	// Handle positioning
	if (anchor & AnchorPosition::LEFT) {
		 current_x = x;
	}
	else if (anchor & AnchorPosition::RIGHT) {
		current_x = x - width;
	}
	else {
		current_x = x - (width / 2.0f);
	}

	if (anchor & AnchorPosition::TOP) {
		current_y = y;
	}
	else if (anchor & AnchorPosition::BOTTOM) {
		current_y = y - height;
	}
	else {
		current_y = y - (height / 2.0f);
	}

	for (uint8_t c : text) {
		// Render character
		font.render_char(c, current_x, current_y);

		// Get width of that character and update current_x
		SDL_Rect char_rect = font.get_character_rect(c);
		current_x += char_rect.w + space_width;
	}
}