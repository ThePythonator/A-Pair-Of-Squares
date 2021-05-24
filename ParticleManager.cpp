#include "ParticleManager.hpp"

void ImageParticle::update(ImageParticle& particle, float dt) {
	particle.x_vel += particle.x_grav * dt;
	particle.y_vel += particle.y_grav * dt;

	particle.x += particle.x_vel * dt;
	particle.y += particle.y_vel * dt;

	particle.angle += particle.spin * dt;
}

void ImageParticle::render(ImageParticle& particle, Spritesheet& spritesheet) {
	spritesheet.sprite(particle.sprite_index, particle.x, particle.y, particle.scale, particle.angle, NULL, SDL_FLIP_NONE);
}


ParticleHandler::ParticleHandler() {

}

void ParticleHandler::update(float dt) {
	for (ImageParticle::ImageParticle& particle : image_particles) {
		ImageParticle::update(particle, dt);
	}
}

void ParticleHandler::render(Spritesheet& spritesheet) {
	for (ImageParticle::ImageParticle& particle : image_particles) {
		ImageParticle::render(particle, spritesheet);
	}
}

void ParticleHandler::add(ImageParticle::ImageParticle particle) {
	image_particles.push_back(particle);
}