#include "ParticleManager.hpp"

ImageParticle::ImageParticle() {

}

ImageParticle::ImageParticle(uint16_t sprite_index, float x, float y, float x_vel, float y_vel, float x_grav, float y_grav, float angle, float spin, float scale) {
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
}

void ImageParticle::update(float dt) {
	x_vel += x_grav * dt;
	y_vel += y_grav * dt;

	x += x_vel * dt;
	y += y_vel * dt;

	angle += spin * dt;
}

void ImageParticle::render(Spritesheet& spritesheet) {
	spritesheet.sprite(sprite_index, x, y, scale, angle, NULL, SDL_FLIP_NONE);
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