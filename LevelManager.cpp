#include "LevelManager.hpp"

Camera::Camera() {

}

Camera::Camera(float x, float y, float ratio, uint16_t screen_mid_x, uint16_t screen_mid_y) {
	this->x = x;
	this->y = y;

	this->ratio = ratio;

	this->screen_mid_x = screen_mid_x;
	this->screen_mid_y = screen_mid_y;
}

void Camera::update(float dt, float player_x, float player_y) {
	x += (player_x - x) * ratio * dt;
	y += (player_y - y) * ratio * dt;
}

float Camera::get_view_x(float x) {
	return x - this->x + screen_mid_x;
}

float Camera::get_view_y(float y) {
	return y - this->y + screen_mid_y;
}


Tile::Tile() {
	sprite_index = 0;

	x = y = 0;
}

Tile::Tile(uint16_t sprite_index, uint16_t x, uint16_t y) {
	this->sprite_index = sprite_index;

	this->x = x;
	this->y = y;
}

void Tile::render(Spritesheet& spritesheet) {
	spritesheet.sprite_scaled(sprite_index, x, y);
}

void Tile::render(Spritesheet& spritesheet, Camera& camera) {
	spritesheet.sprite_scaled(sprite_index, camera.get_view_x(x), camera.get_view_y(y));
}

uint16_t Tile::get_x() {
	return x;
}

uint16_t Tile::get_y() {
	return y;
}


LevelHandler::LevelHandler() {
	sprite_size = 0;

	level_spawn_blue_x = 0;
	level_spawn_blue_y = 0;
	level_spawn_pink_x = 0;
	level_spawn_pink_y = 0;
	level_finish_blue_x = 0;
	level_finish_blue_y = 0;
	level_finish_pink_x = 0;
	level_finish_pink_y = 0;
}

LevelHandler::LevelHandler(uint8_t sprite_size) {
	this->sprite_size = sprite_size;

	level_spawn_blue_x = 0;
	level_spawn_blue_y = 0;
	level_spawn_pink_x = 0;
	level_spawn_pink_y = 0;
	level_finish_blue_x = 0;
	level_finish_blue_y = 0;
	level_finish_pink_x = 0;
	level_finish_pink_y = 0;
}


void LevelHandler::load_level(const uint8_t level_data[]) {
	level_spawn_blue_x = 0;
	level_spawn_blue_y = 0;
	level_spawn_pink_x = 0;
	level_spawn_pink_y = 0;
	level_finish_blue_x = 0;
	level_finish_blue_y = 0;
	level_finish_pink_x = 0;
	level_finish_pink_y = 0;

	tiles.clear();

	TMX* level_tmx = (TMX*)level_data;

	for (uint16_t i = 0; i < level_tmx->height * level_tmx->width; i++) {
		if (level_tmx->data[i] != level_tmx->empty_tile) {
			// TODO: remove literals
			if (level_tmx->data[i] == 0) {
				// Blue spawn
				level_spawn_blue_x = (i % level_tmx->width) * sprite_size;
				level_spawn_blue_y = (i / level_tmx->width) * sprite_size;
			}
			else if (level_tmx->data[i] == 4) {
				// Pink spawn
				level_spawn_pink_x = (i % level_tmx->width) * sprite_size;
				level_spawn_pink_y = (i / level_tmx->width) * sprite_size;
			}
			else if (level_tmx->data[i] == 96) {
				// Finish spawn for blue
				level_finish_blue_x = (i % level_tmx->width) * sprite_size;
				level_finish_blue_y = (i / level_tmx->width) * sprite_size;
			}
			else if (level_tmx->data[i] == 97) {
				// Finish spawn for pink
				level_finish_pink_x = (i % level_tmx->width) * sprite_size;
				level_finish_pink_y = (i / level_tmx->width) * sprite_size;
			}
			else {
				tiles.push_back(Tile(level_tmx->data[i], (i % level_tmx->width) * sprite_size, (i / level_tmx->width) * sprite_size));
			}
		}
	}
}

//void LevelHandler::update(float dt) {
//
//}

void LevelHandler::render(Spritesheet& spritesheet) {
	for (Tile& tile : tiles) {
		tile.render(spritesheet);
	}

	// Render finish
	spritesheet.sprite_scaled(96, level_finish_blue_x, level_finish_blue_y);
	spritesheet.sprite_scaled(97, level_finish_pink_x, level_finish_pink_y);
}

void LevelHandler::render(Spritesheet& spritesheet, Camera& camera) {
	for (Tile& tile : tiles) {
		tile.render(spritesheet, camera);
	}

	// Render finish
	spritesheet.sprite_scaled(96, camera.get_view_x(level_finish_blue_x), camera.get_view_y(level_finish_blue_y));
	spritesheet.sprite_scaled(97, camera.get_view_x(level_finish_pink_x), camera.get_view_y(level_finish_pink_y));
}

std::vector<Tile> LevelHandler::get_tiles() {
	return tiles;
}

uint8_t LevelHandler::get_sprite_size() {
	return sprite_size;
}