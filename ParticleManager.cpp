#include "ParticleManager.hpp"

ImageParticle::ImageParticle() {

}

ImageParticle::ImageParticle(uint16_t sprite_index, float x, float y, float x_vel, float y_vel, float x_grav, float y_grav, float angle, float spin, float scale, float alpha, float fade) {
	this->sprite_index = sprite_index;
	
	this->x = x;
	this->y = y;

	this->x_vel = x_vel;
	this->y_vel = y_vel;

	this->x_grav = x_grav;
	this->y_grav = y_grav;

	this->angle = angle;
	this->spin = spin;

	this->scale = scale;

	this->alpha = alpha;
	this->fade = fade;
}

void ImageParticle::update(float dt) {
	x_vel += x_grav * dt;
	y_vel += y_grav * dt;

	x += x_vel * dt;
	y += y_vel * dt;

	angle += spin * dt;

	alpha += fade * dt;

	if (alpha > 255.0f) {
		alpha = 255.0f;
	}
	else if (alpha < 0.0f) {
		alpha = 0.0f;
	}
}

void ImageParticle::render(Spritesheet& spritesheet) {
	uint8_t old_alpha = spritesheet.get_alpha();
	spritesheet.set_alpha(alpha);
	spritesheet.sprite(sprite_index, x, y, scale, angle, NULL, SDL_FLIP_NONE);
	spritesheet.set_alpha(old_alpha);
}

float ImageParticle::get_x() {
	return x;
}

float ImageParticle::get_y() {
	return y;
}

float ImageParticle::get_scale() {
	return scale;
}

float ImageParticle::get_alpha() {
	return alpha;
}


ParticleHandler::ParticleHandler() {

}

void ParticleHandler::update(float dt) {
	for (ImageParticle& particle : image_particles) {
		particle.update(dt);
	}
}

void ParticleHandler::render(Spritesheet& spritesheet) {
	for (ImageParticle& particle : image_particles) {
		particle.render(spritesheet);
	}
}

void ParticleHandler::add(ImageParticle particle) {
	image_particles.push_back(particle);
}

void ParticleHandler::remove_if(bool (*condition) (ImageParticle&)) {
	image_particles.erase(std::remove_if(image_particles.begin(), image_particles.end(), condition), image_particles.end());
}

void ParticleHandler::clear() {
	image_particles.clear();
}

uint16_t ParticleHandler::count_particles() {
	return image_particles.size();
}