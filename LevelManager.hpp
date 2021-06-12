#pragma once

#include <vector>

#include "Spritesheet.hpp"

// Struct header generated for the tmx files
#pragma pack(push,1)
struct TMX {
	char head[4];
	uint16_t header_length;
	uint16_t flags;
	uint16_t empty_tile;
	uint16_t width;
	uint16_t height;
	uint16_t layers;
	uint8_t data[];
};
#pragma pack(pop)

class Camera {
public:
	Camera();
	Camera(float x, float y, float ratio);//, uint16_t screen_mid_x, uint16_t screen_mid_y

	void update(float dt, float player_x, float player_y);

	float get_view_x(float x);
	float get_view_y(float y);

private:
	float x = 0.0f;
	float y = 0.0f;

	float ratio = 1.0f;

	/*uint16_t screen_mid_x = 0;
	uint16_t screen_mid_y = 0;*/
};

class Tile {
public:
	Tile();
	Tile(uint16_t sprite_index, uint16_t x, uint16_t y);

	void render(Spritesheet& spritesheet, Camera& camera);

	uint16_t get_x();
	uint16_t get_y();

private:
	uint16_t sprite_index;
	uint16_t x, y;
};

class LevelHandler {
public:
	uint8_t sprite_size;

	uint16_t level_spawn_pink_x, level_spawn_pink_y, level_spawn_blue_x, level_spawn_blue_y, level_finish_x, level_finish_y;

	LevelHandler();

	void load_level(const uint8_t level_data[], uint8_t sprite_size);

	//void update(float dt);
	void render(Spritesheet& spritesheet, Camera& camera);

	std::vector<Tile> get_tiles();

private:
	std::vector<Tile> tiles;
};

// TODO: add method of loading tmx data into Level