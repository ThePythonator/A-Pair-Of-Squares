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
	orbs.clear();
	spikes.clear();
	springs.clear();
	buttons.clear();
	doors.clear();

	TMX* level_tmx = (TMX*)level_data;
	uint16_t tile_id, x, y;

	for (uint16_t i = 0; i < level_tmx->height * level_tmx->width; i++) {
		tile_id = level_tmx->data[i];
		x = (i % level_tmx->width) * sprite_size;
		y = (i / level_tmx->width) * sprite_size;

		if (tile_id != level_tmx->empty_tile) {
			if (tile_id == TILE_ID::PLAYER::BLUE) {
				// Blue spawn
				level_spawn_blue_x = x;
				level_spawn_blue_y = y;
			}
			else if (tile_id == TILE_ID::PLAYER::PINK) {
				// Pink spawn
				level_spawn_pink_x = x;
				level_spawn_pink_y = y;
			}

			else if (tile_id == TILE_ID::FINISH::BLUE) {
				// Finish spawn for blue
				level_finish_blue_x = x;
				level_finish_blue_y = y;
			}
			else if (tile_id == TILE_ID::FINISH::PINK) {
				// Finish spawn for pink
				level_finish_pink_x = x;
				level_finish_pink_y = y;
			}
			else if (tile_id == TILE_ID::ORB::BLUE) {
				// Blue orb
				orbs.push_back(Orb(tile_id, 0, x, y));
			}
			else if (tile_id == TILE_ID::ORB::PINK) {
				// Pink Orb
				orbs.push_back(Orb(tile_id, 1, x, y));
			}
			else if (
				tile_id == TILE_ID::SPIKE::DOUBLE_BOTTOM ||
				tile_id == TILE_ID::SPIKE::DOUBLE_LEFT ||
				tile_id == TILE_ID::SPIKE::DOUBLE_TOP ||
				tile_id == TILE_ID::SPIKE::DOUBLE_RIGHT ||

				tile_id == TILE_ID::SPIKE::SINGLE_BOTTOM_LEFT ||
				tile_id == TILE_ID::SPIKE::SINGLE_BOTTOM_RIGHT ||

				tile_id == TILE_ID::SPIKE::SINGLE_LEFT_TOP ||
				tile_id == TILE_ID::SPIKE::SINGLE_LEFT_BOTTOM ||

				tile_id == TILE_ID::SPIKE::SINGLE_TOP_LEFT ||
				tile_id == TILE_ID::SPIKE::SINGLE_TOP_RIGHT ||

				tile_id == TILE_ID::SPIKE::SINGLE_RIGHT_TOP ||
				tile_id == TILE_ID::SPIKE::SINGLE_RIGHT_BOTTOM) {

				// Spike
				spikes.push_back(Spike(tile_id, parse_spike_id(tile_id), x, y));
			}
			else if (tile_id == TILE_ID::SPRING::BASE_FRAME) {
				// Spring
				springs.push_back(Spring(tile_id, x, y));
			}
			else if (tile_id == TILE_ID::BUTTON::BLUE_RELEASED) {
				// Button
				buttons.push_back(Button(tile_id, x, y, 0));
			}
			else if (tile_id == TILE_ID::BUTTON::PINK_RELEASED) {
				// Button
				buttons.push_back(Button(tile_id, x, y, 1));
			}
			else if (tile_id == TILE_ID::DOOR::VERTICAL::BLUE_MIDDLE) {
				// Door
				doors.push_back(Door(tile_id, x, y, 0));
			}
			else if (tile_id == TILE_ID::DOOR::VERTICAL::PINK_MIDDLE) {
				// Door
				doors.push_back(Door(tile_id, x, y, 1));
			}
			else if (
				tile_id == TILE_ID::DOOR::VERTICAL::BLUE_TOP ||
				tile_id == TILE_ID::DOOR::VERTICAL::BLUE_BOTTOM ||

				tile_id == TILE_ID::DOOR::VERTICAL::PINK_TOP ||
				tile_id == TILE_ID::DOOR::VERTICAL::PINK_BOTTOM) {

				// Blank tile (don't render and don't collide)
				// Don't do anything
			}
			else {
				// Normal tile
				tiles.push_back(Tile(tile_id, x, y));
			}
		}
	}
}

void LevelHandler::update(float dt) { //Player& player,
	for (Orb& orb : orbs) {
		orb.update(dt);
	}

	for (Spring& spring : springs) {
		spring.update(dt);
	}

	bool blue_pressed = false;
	bool pink_pressed = false;

	for (Button& button : buttons) {
		button.update(dt);
		if (button.get_pressed()) {
			if (button.get_type() == 0) {
				blue_pressed = true;
			}
			else {
				pink_pressed = true;
			}
		}
	}

	for (Door& door : doors) {
		if ((blue_pressed && door.get_type() == 0) ||
			(pink_pressed && door.get_type() == 1)) {

			if (!door.is_open() && !door.is_opening()) {
				door.open();
			}
		}
		else {
			door.close();
		}

		door.update(dt);
	}
}

void LevelHandler::render(Spritesheet& spritesheet) {
	for (Door& door : doors) {
		door.render(spritesheet);
	}

	for (Tile& tile : tiles) {
		tile.render(spritesheet);
	}

	for (Orb& orb : orbs) {
		orb.render(spritesheet);
	}

	for (Spike& spike : spikes) {
		spike.render(spritesheet);
	}
	
	for (Spring& spring : springs) {
		spring.render(spritesheet);
	}

	for (Button& button : buttons) {
		button.render(spritesheet);
	}

	// Render finish
	spritesheet.sprite_scaled(TILE_ID::FINISH::BLUE, level_finish_blue_x, level_finish_blue_y);
	spritesheet.sprite_scaled(TILE_ID::FINISH::PINK, level_finish_pink_x, level_finish_pink_y);
}

//void LevelHandler::render(Spritesheet& spritesheet, Camera& camera) {
//	for (Tile& tile : tiles) {
//		tile.render(spritesheet, camera);
//	}
//
//	// Render finish
//	spritesheet.sprite_scaled(TILE_ID::FINISH::BLUE, camera.get_view_x(level_finish_blue_x), camera.get_view_y(level_finish_blue_y));
//	spritesheet.sprite_scaled(TILE_ID::FINISH::PINK, camera.get_view_x(level_finish_pink_x), camera.get_view_y(level_finish_pink_y));
//}

uint8_t LevelHandler::handle_orb_collisions(float x, float y, uint8_t type) {
	// Handle star collisions
	uint8_t count = 0;

	for (Orb& orb : orbs) {
		// Orb hasn't been collected and is same colour as player
		if (!orb.get_collected() && type == orb.get_type()) {
			// Orb is colliding
			if (is_colliding(orb.get_x() + GAME::ORB::BORDER, orb.get_y() + GAME::ORB::BORDER, x, y, GAME::ORB::WIDTH, GAME::ORB::WIDTH, sprite_size, sprite_size)) {
				orb.set_collected();
				count++; // Note: sometimes (rarely) player gets awarded many orbs when there aren't that many in the level
				//printf("orb %u, %u, collected: %u\n", type, orb.get_type(), orb.get_collected());

				//TODO: create particle or something with same image which rotates, grows larger and fades at location of orb just collected
			}
		}
	}

	return count;
}

bool LevelHandler::handle_spike_collisions(float x, float y) {
	for (Spike& spike : spikes) {
		// Spike is colliding
		if (spike.check_collision(x, y)) {
			return true;

			//TODO: create something to show player was killed
		}
	}

	return false;
}

std::vector<Tile>& LevelHandler::get_tiles() {
	return tiles;
}

//std::vector<Orb> LevelHandler::get_orbs() {
//	return orbs;
//}

std::vector<Spring>& LevelHandler::get_springs() {
	return springs;
}

std::vector<Button>& LevelHandler::get_buttons() {
	return buttons;
}

std::vector<Door>& LevelHandler::get_doors() {
	return doors;
}



uint8_t LevelHandler::get_sprite_size() {
	return sprite_size;
}



bool is_colliding(float tile_x, float tile_y, float x, float y, uint8_t tile_w, uint8_t tile_h, uint8_t w, uint8_t h) {
	return tile_x + tile_w > x && tile_x < x + w && tile_y + tile_h > y && tile_y < y + h;
}


bool is_colliding(Tile& tile, float x, float y, uint8_t sprite_size) {
	return (tile.get_x() + sprite_size > x && tile.get_x() < x + sprite_size && tile.get_y() + sprite_size > y && tile.get_y() < y + sprite_size);
}

bool is_colliding(float tile_x, float tile_y, float x, float y, uint8_t sprite_size) {
	return (tile_x + sprite_size > x && tile_x < x + sprite_size && tile_y + sprite_size > y && tile_y < y + sprite_size);
}


bool check_on_top(float tile_x, float tile_y, float x, float y, uint8_t tile_w, uint8_t tile_h, uint8_t w, uint8_t h) {
	// Checks if on top of tile
	return tile_x + tile_w > x && tile_x < x + w && tile_y == y + h;
}

bool check_on_top(Tile& tile, float x, float y, uint8_t sprite_size) {
	return (tile.get_x() + sprite_size > x && tile.get_x() < x + sprite_size && tile.get_y() == y + sprite_size);
}


Spike::SpikeDirection parse_spike_id(uint16_t spike_id) {
	if (spike_id == TILE_ID::SPIKE::DOUBLE_BOTTOM) {
		return Spike::SpikeDirection::DOUBLE_BOTTOM;
	}
	else if (spike_id == TILE_ID::SPIKE::DOUBLE_LEFT) {
		return Spike::SpikeDirection::DOUBLE_LEFT;
	}
	else if (spike_id == TILE_ID::SPIKE::DOUBLE_TOP) {
		return Spike::SpikeDirection::DOUBLE_TOP;
	}
	else if (spike_id == TILE_ID::SPIKE::DOUBLE_RIGHT) {
		return Spike::SpikeDirection::DOUBLE_RIGHT;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_BOTTOM_LEFT) {
		return Spike::SpikeDirection::SINGLE_BOTTOM_LEFT;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_BOTTOM_RIGHT) {
		return Spike::SpikeDirection::SINGLE_BOTTOM_RIGHT;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_LEFT_TOP) {
		return Spike::SpikeDirection::SINGLE_LEFT_TOP;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_LEFT_BOTTOM) {
		return Spike::SpikeDirection::SINGLE_LEFT_BOTTOM;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_TOP_LEFT) {
		return Spike::SpikeDirection::SINGLE_TOP_LEFT;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_TOP_RIGHT) {
		return Spike::SpikeDirection::SINGLE_TOP_RIGHT;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_RIGHT_TOP) {
		return Spike::SpikeDirection::SINGLE_RIGHT_TOP;
	}
	else if (spike_id == TILE_ID::SPIKE::SINGLE_RIGHT_BOTTOM) {
		return Spike::SpikeDirection::SINGLE_RIGHT_BOTTOM;
	}
	else {
		// Pick one
		return Spike::SpikeDirection::DOUBLE_BOTTOM;
	}
}