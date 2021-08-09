#pragma once

#include "SDL.h"

#include <vector>

#include "Spritesheet.hpp"

// No direct support for Filled polygons
//namespace ShapeParticle {
//	struct ShapeParticle {
//		float x, y, x_vel, y_vel;
//		float x_grav, y_grav;
//
//		uint8_t sides;
//		float angle;
//
//		//COLOUR colour;
//		bool filled;
//	};
//}

class ImageParticle {
public:
	ImageParticle();
	ImageParticle(uint16_t sprite_index, float x, float y, float x_vel = 0.0f, float y_vel = 0.0f, float x_grav = 0.0f, float y_grav = 0.0f, float angle = 0.0f, float spin = 0.0f, float scale = 1.0f, float alpha = 255.0f, float fade = 0.0f);

	void update(float dt);

	void render(Spritesheet& spritesheet);

	float get_x();
	float get_y();

	float get_scale();

	float get_alpha();

private:
	uint16_t sprite_index = 0;

	float x = 0.0f;
	float y = 0.0f;

	float x_vel = 0.0f;
	float y_vel = 0.0f;

	float x_grav = 0.0f;
	float y_grav = 0.0f;

	float angle = 0.0f;
	float spin = 0.0f;

	float scale = 1.0f;

	float alpha = 255.0f;
	float fade = 0.0f;
};

class ParticleHandler {
public:
	ParticleHandler();

	void update(float dt);

	void render(Spritesheet& spritesheet);

	//void add(ShapeParticle::ShapeParticle);
	void add(ImageParticle particle);
	void remove_if(bool (*condition) (ImageParticle&));
	void clear();

	uint16_t count_particles();

private:
	std::vector<ImageParticle> image_particles;
};