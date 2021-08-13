#include "LevelManager.hpp"

namespace TILE_ID {
	namespace PLAYER {
		const uint16_t BLUE = 0;
		const uint16_t PINK = 4;
	}

	namespace TILE {
		//const uint16_t STAR = 8;
	}

	namespace FINISH {
		const uint16_t BLUE = 96;
		const uint16_t PINK = 97;
	}

	namespace ORB {
		const uint16_t BLUE = 98;
		const uint16_t PINK = 99;
	}
}

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
			if (level_tmx->data[i] == TILE_ID::PLAYER::BLUE) {
				// Blue spawn
				level_spawn_blue_x = (i % level_tmx->width) * sprite_size;
				level_spawn_blue_y = (i / level_tmx->width) * sprite_size;
			}
			else if (level_tmx->data[i] == TILE_ID::PLAYER::PINK) {
				// Pink spawn
				level_spawn_pink_x = (i % level_tmx->width) * sprite_size;
				level_spawn_pink_y = (i / level_tmx->width) * sprite_size;
			}
			else if (level_tmx->data[i] == TILE_ID::FINISH::BLUE) {
				// Finish spawn for blue
				level_finish_blue_x = (i % level_tmx->width) * sprite_size;
				level_finish_blue_y = (i / level_tmx->width) * sprite_size;
			}
			else if (level_tmx->data[i] == TILE_ID::FINISH::PINK) {
				// Finish spawn for pink
				level_finish_pink_x = (i % level_tmx->width) * sprite_size;
				level_finish_pink_y = (i / level_tmx->width) * sprite_size;
			}
			else if (level_tmx->data[i] == TILE_ID::ORB::BLUE) {
				// Star
				orbs.push_back(Orb(level_tmx->data[i], 0, (i % level_tmx->width) * sprite_size, (i / level_tmx->width) * sprite_size));
			}
			else if (level_tmx->data[i] == TILE_ID::ORB::PINK) {
				// Star
				orbs.push_back(Orb(level_tmx->data[i], 1, (i % level_tmx->width) * sprite_size, (i / level_tmx->width) * sprite_size));
			}
			else {
				tiles.push_back(Tile(level_tmx->data[i], (i % level_tmx->width) * sprite_size, (i / level_tmx->width) * sprite_size));
			}
		}
	}
}

//void LevelHandler::update(Player& player, float dt) {
//	// todo: check if player is colliding with any star
//}

void LevelHandler::render(Spritesheet& spritesheet) {
	for (Tile& tile : tiles) {
		tile.render(spritesheet);
	}

	for (Orb& orb : orbs) {
		orb.render(spritesheet);
	}

	// Render finish
	spritesheet.sprite_scaled(TILE_ID::FINISH::BLUE, level_finish_blue_x, level_finish_blue_y);
	spritesheet.sprite_scaled(TILE_ID::FINISH::PINK, level_finish_pink_x, level_finish_pink_y);
}

void LevelHandler::render(Spritesheet& spritesheet, Camera& camera) {
	for (Tile& tile : tiles) {
		tile.render(spritesheet, camera);
	}

	// Render finish
	spritesheet.sprite_scaled(TILE_ID::FINISH::BLUE, camera.get_view_x(level_finish_blue_x), camera.get_view_y(level_finish_blue_y));
	spritesheet.sprite_scaled(TILE_ID::FINISH::PINK, camera.get_view_x(level_finish_pink_x), camera.get_view_y(level_finish_pink_y));
}

uint8_t LevelHandler::handle_orb_collisions(float x, float y, uint8_t type) {
	// Handle star collisions
	uint8_t count = 0;

	for (Orb& orb : orbs) {
		// Orb hasn't been collected and is same colour as player
		if (!orb.get_collected() && type == orb.get_type()) {
			// Orb is colliding
			if (is_colliding(orb.get_x(), orb.get_y(), x, y, sprite_size)) {
				orb.set_collected();
				count++;
				//printf("orb %u, %u, collected: %u\n", type, orb.get_type(), orb.get_collected());
			}
		}
	}

	return count;
}

std::vector<Tile> LevelHandler::get_tiles() {
	return tiles;
}

//std::vector<Orb> LevelHandler::get_orbs() {
//	return orbs;
//}

uint8_t LevelHandler::get_sprite_size() {
	return sprite_size;
}



bool is_colliding(Tile& tile, float x, float y, uint8_t sprite_size) {
	return (tile.get_x() + sprite_size > x && tile.get_x() < x + sprite_size && tile.get_y() + sprite_size > y && tile.get_y() < y + sprite_size);
}

bool is_colliding(float tile_x, float tile_y, float x, float y, uint8_t sprite_size) {
	return (tile_x + sprite_size > x && tile_x < x + sprite_size && tile_y + sprite_size > y && tile_y < y + sprite_size);
}

bool is_on_tile(Tile& tile, float x, float y, uint8_t sprite_size) {
	return (tile.get_x() + sprite_size > x && tile.get_x() < x + sprite_size && tile.get_y() == y + sprite_size);
}