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

namespace ImageParticle {
	struct ImageParticle {
		uint8_t sprite_index;

		float x, y;
		float x_vel, y_vel;
		float x_grav, y_grav;

		float angle;
		float spin;

		float scale;
	};

	void update(ImageParticle& particle, float dt);
	
	void render(ImageParticle& particle, Spritesheet& spritesheet);
}

class ParticleHandler {
public:
	ParticleHandler();

	void update(float dt);

	void render(Spritesheet& spritesheet);

	//void add(ShapeParticle::ShapeParticle);
	void add(ImageParticle::ImageParticle particle);

protected:
	std::vector<ImageParticle::ImageParticle> image_particles;
};